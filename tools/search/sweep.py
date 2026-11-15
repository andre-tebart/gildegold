"""Sweep: climb.py over every function below byte, the nearest first; resumable.

    python tools/search/sweep.py                    # one exhaustive round (every single mutation) per function
    python tools/search/sweep.py --rounds 3         # deeper
    python tools/search/sweep.py --max-ins 60       # only functions at most this far
    python tools/search/sweep.py --unit main        # one unit
    python tools/search/sweep.py --apply            # write each byte hit into the source at once (see below)
    python tools/search/sweep.py --report           # the results so far: byte hits, gains

Results go to build/climb/sweep.jsonl (one line per function: start and best distance, the mutation path) and the
best text of each function to build/climb/NAME.best.c. Without --apply nothing is written into the sources: a
byte hit is a lead that a worker applies as plain code (`python tools/search/climb.py NAME --rounds 1` prints the
diff again). With --apply a byte hit is written into the source with `dcp try --apply` (the function's comment
is kept) and listed in build/climb/applied.txt; `dcp build` and the commit are yours, and so is the tidying a hit
can need: a local the search introduced is called t / v / k (name it), a local it wrote out leaves an unused
declaration (remove it), parentheses it added around a written-out expression may be droppable. Only byte hits
are applied: a hit that is only closer may have changed the meaning. A function already in the results is skipped
unless its distance in the tree changed since. climb.py never lets a mutant win by changing the frame size (see
there); a function whose tree text already has another frame size than the original's is flagged `frame` in the
results.
"""
import argparse, json, os, re, subprocess, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.getcwd()
while not os.path.exists(os.path.join(ROOT, "decompanion.toml")) and os.path.dirname(ROOT) != ROOT:
    ROOT = os.path.dirname(ROOT)
OUT = os.path.join(ROOT, "build", "climb")
LOG = os.path.join(OUT, "sweep.jsonl")


def dcp(*args):
    r = subprocess.run(["dcp"] + list(args), cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    return r.stdout + r.stderr


def queue(unit):
    out = []
    for q in ("--deep", "--base"):
        for line in dcp("next", q, "-n", "5000").split("\n"):
            m = re.search(r"name=(\S+)\s+unit=(\S+).*dist='(\d+) ins / (\d+) bytes", line)
            if m and (not unit or m.group(2) == unit) and m.group(1) not in [o[0] for o in out]:
                out.append((m.group(1), m.group(2), int(m.group(3)), int(m.group(4))))
    return out


def results():
    res = {}
    if os.path.exists(LOG):
        for l in open(LOG, encoding="utf-8"):
            if l.strip():
                r = json.loads(l)
                res[r["name"]] = r
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--rounds", type=int, default=1)
    ap.add_argument("--beam", type=int, default=6)
    ap.add_argument("--width", type=int, default=120)
    ap.add_argument("--max-ins", type=int, default=100000)
    ap.add_argument("--unit")
    ap.add_argument("--report", action="store_true")
    ap.add_argument("--apply", action="store_true", help="write each byte hit into the source")
    a = ap.parse_args()
    os.makedirs(OUT, exist_ok=True)
    res = results()
    if a.report:
        hits = [r for r in res.values() if r["best"][:2] == [0, 0]]
        gains = sorted([r for r in res.values() if r["best"][:2] != [0, 0] and r["best"][0] < r["start"][0] and not r.get("frame")], key=lambda r: r["best"][0] - r["start"][0])
        print("%d functions swept, %d byte, %d closer" % (len(res), len(hits), len(gains)))
        for r in hits:
            print("  BYTE  %-34s %-10s %4d -> 0    %s" % (r["name"], r["unit"], r["start"][0], " > ".join(r["path"])))
        for r in gains:
            print("        %-34s %-10s %4d -> %-4d %s" % (r["name"], r["unit"], r["start"][0], r["best"][0], " > ".join(r["path"])))
        return
    todo = sorted(queue(a.unit), key=lambda q: (q[2], q[3]))
    todo = [q for q in todo if q[2] <= a.max_ins and not (q[0] in res and res[q[0]]["start"][:2] == [q[2], q[3]] and res[q[0]].get("rounds", 1) >= a.rounds)]
    print("%d functions to sweep" % len(todo))
    for n, (name, unit, ins, by) in enumerate(todo, 1):
        text = ""
        for attempt in range(40):
            r = subprocess.run([sys.executable, os.path.join(HERE, "climb.py"), name, "--rounds", str(a.rounds), "--beam", str(a.beam), "--width", str(a.width)],
                               cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
            text = r.stdout + r.stderr
            if "\nbest: " in text or "does not compile" in text:
                break
            time.sleep(60)                                # dcp is being reinstalled or the daemon restarts: wait
        m = re.search(r"\nbest: (\d+) ins / (\d+) bytes.*?path: (.*)", text)
        s = re.search(r"start (\d+) ins / (\d+) bytes", text)
        if not m or not s:
            print("%4d/%d %-34s failed: %s" % (n, len(todo), name, text.strip().split("\n")[-1][:120]))
            continue
        rec = dict(name=name, unit=unit, start=[int(s.group(1)), int(s.group(2))], best=[int(m.group(1)), int(m.group(2))],
                   path=[] if m.group(3).strip() == "(start)" else m.group(3).strip().split(" > "), rounds=a.rounds)
        f = re.search(r"frame (\S+ vs \S+)", m.group(0))
        if f:
            rec["frame"] = f.group(1)                     # the best text's frame size is not the original's
        with open(LOG, "a", encoding="utf-8") as f:
            f.write(json.dumps(rec) + "\n")
        if a.apply and rec["best"] == [0, 0] and rec["path"]:
            head = dcp("fn", name).split("\n")[0]
            ad = re.search(r"fn=(0x[0-9a-f]+)", head)
            body = open(os.path.join(OUT, name + ".best.c"), encoding="latin-1").read()
            vf = os.path.join(OUT, name + ".apply.txt")
            open(vf, "w", encoding="latin-1", newline="\n").write("### best\n/* 0x%x, x */\n%s" % (int(ad.group(1), 16), body.lstrip("\n")))
            done = dcp("try", name, vf, "--apply", "best")
            os.unlink(vf)
            ok = "replaced by variant" in done
            tidy = [p.split("@")[0] for p in rec["path"] if p.startswith(("local:", "inline:"))]
            with open(os.path.join(OUT, "applied.txt"), "a", encoding="utf-8") as f:
                f.write("%s\t%s\t%s\t%s\n" % (name, unit, "applied" if ok else "NOT applied", ("tidy: " + ", ".join(tidy)) if tidy else ""))
        flag = "BYTE" if rec["best"] == [0, 0] else "frame" if rec.get("frame") else ("closer" if rec["best"][0] < rec["start"][0] else "")
        print("%4d/%d %-34s %4d -> %-4d %-6s %s" % (n, len(todo), name, rec["start"][0], rec["best"][0], flag, " > ".join(rec["path"])))
        sys.stdout.flush()


if __name__ == "__main__":
    main()
