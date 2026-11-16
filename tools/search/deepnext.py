"""deepnext: the nearest deep item across all units (`dcp next --deep` hands out in its own order, which reaches
300-900 instruction functions while other units hold items 2 instructions from byte).

    python tools/search/deepnext.py [-n 10]

Prints each unit's deep head (`dcp next --deep --peek --unit U`), nearest first (instructions, then bytes, fewest
unfinished attempts first). Take the first with `dcp next --deep --unit U` (that call records it as the session's
item); nothing here records anything.
"""
import argparse, os, re, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def units():
    out = set()
    with open(os.path.join(ROOT, "ledger", "grades.tsv"), encoding="utf-8") as f:
        for l in f:
            p = l.rstrip("\n").split("\t")
            if len(p) >= 5 and p[0] != "target" and p[4] != "byte":
                out.add(p[3])
    return sorted(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("-n", type=int, default=10)
    a = ap.parse_args()
    rows = []
    for u in units():
        r = subprocess.run(["dcp", "next", "--deep", "--peek", "--unit", u], cwd=ROOT, capture_output=True, text=True)
        m = re.search(r"^next=\S+\s+name=(\S+).*?attempts=(\d+).*?dist='(\d+) ins / (\d+) bytes", r.stdout, re.M)
        if m:
            rows.append((int(m.group(2)), int(m.group(3)), int(m.group(4)), m.group(1), u))
    rows.sort(key=lambda t: (t[0], t[1], t[2]))
    for att, ins, by, name, u in rows[:a.n]:
        print("%-36s unit=%-14s %4d ins %5d bytes  attempts=%d   take: dcp next --deep --unit %s" % (name, u, ins, by, att, u))
    print("%d units with deep items" % len(rows))


if __name__ == "__main__":
    main()
