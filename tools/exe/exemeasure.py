"""exemeasure: how far the linked exe is from the original, as a whole (not per function).

    python tools/exe/exemeasure.py                 # summary: sections, functions at their address, first mismatches
    python tools/exe/exemeasure.py --queue [-n N]  # the size queue: functions whose padded size differs, by address
    python tools/exe/exemeasure.py --unit ch       # the size queue of one unit
    python tools/exe/exemeasure.py --no-link       # use build/link/exe as it is (default: `dcp link` first)

Why: per-function byte matching does not move a function to its original address. One function 16 bytes short
shifts everything after it, so the exe only lines up when every function has the original's padded size (its
distance to the next game function, alignment included) and the original's order. The size queue is the work
list for that: fix the lowest address first, every function after it lines up up to the next mismatch. A size fix
needs the right instruction lengths, not the right bytes (but the right bytes are the usual way to get there).

Sizes come from build/link/exe/exe.map (ours) and ledger/grades.tsv's addresses (the original); a function's
size is its distance to the next game function present on both sides.
"""
import argparse, csv, os, re, struct, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
OURS = os.path.join(ROOT, "build", "link", "exe", "GildeGold.exe")
MAP = os.path.join(ROOT, "build", "link", "exe", "exe.map")


def game_dir():
    for name in ("decompanion.local.toml", "decompanion.toml"):
        p = os.path.join(ROOT, name)
        if os.path.exists(p):
            m = re.search(r'^game\s*=\s*"([^"]+)"', open(p, encoding="utf-8").read(), re.M)
            if m:
                return m.group(1)
    sys.exit("exemeasure: no [paths] game in decompanion.toml")


def sections(path):
    d = open(path, "rb").read()
    o = struct.unpack_from("<I", d, 0x3c)[0]
    n = struct.unpack_from("<H", d, o + 6)[0]
    opt = struct.unpack_from("<H", d, o + 20)[0]
    ep = struct.unpack_from("<I", d, o + 24 + 16)[0]
    s = {}
    for i in range(n):
        b = o + 24 + opt + 40 * i
        vs, va, rs, rp = struct.unpack_from("<IIII", d, b + 8)
        s[d[b:b + 8].rstrip(b"\0").decode()] = (va, vs, d[rp:rp + rs])
    return d, s, ep


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--queue", action="store_true")
    ap.add_argument("--unit")
    ap.add_argument("-n", type=int, default=20)
    ap.add_argument("--no-link", action="store_true")
    a = ap.parse_args()
    if not a.no_link:
        r = subprocess.run(["dcp", "link"], cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
        if "ok=yes" not in r.stdout:
            sys.exit("exemeasure: dcp link failed:\n" + r.stdout[-1500:] + r.stderr[-1500:])
    g = {}
    with open(os.path.join(ROOT, "ledger", "grades.tsv"), encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            g[row["name"]] = (int(row["address"], 16), row["unit"], row["code"])
    m = {}
    for l in open(MAP, encoding="latin-1"):
        x = re.match(r"\s*0001:[0-9a-f]+\s+_?(\S+)\s+([0-9a-f]{8})", l)
        if x and x.group(1) in g:
            m[x.group(1)] = int(x.group(2), 16)
    both = [n for n in g if n in m]
    orig = sorted(both, key=lambda n: g[n][0])
    ours = sorted(both, key=lambda n: m[n])
    osz = {n: g[orig[i + 1]][0] - g[n][0] for i, n in enumerate(orig[:-1])}
    usz = {n: m[ours[i + 1]] - m[n] for i, n in enumerate(ours[:-1])}
    oidx = {n: i for i, n in enumerate(orig)}
    misorder = [(ours[i], ours[i + 1]) for i in range(len(ours) - 1) if oidx[ours[i]] > oidx[ours[i + 1]]]
    # a function counts as out of order when its successor in our link is not its successor in the original
    seq_bad = {x for x, y in zip(ours, ours[1:]) if oidx[y] != oidx[x] + 1}
    sizeq = [n for n in orig if n in usz and n in osz and osz[n] != usz[n] and n not in seq_bad]
    placed = sum(1 for n in both if m[n] == g[n][0])

    if a.queue or a.unit:
        rows = [n for n in sizeq if not a.unit or g[n][1] == a.unit]
        print("size queue: %d functions%s (lowest address first)" % (len(rows), " in " + a.unit if a.unit else ""))
        for n in rows[:a.n]:
            print("  0x%08x %-34s unit=%-10s %-6s orig=%d ours=%d (%+d)" % (
                g[n][0], n, g[n][1], g[n][2], osz[n], usz[n], usz[n] - osz[n]))
        return

    od, osec, oep = sections(os.path.join(game_dir(), "GildeGold.exe"))
    ud, usec, uep = sections(OURS)
    print("exe: file %d vs ours %d bytes, identical=%s, entry 0x%x vs 0x%x" % (len(od), len(ud), od == ud, oep, uep))
    for k, (va, vs, raw) in osec.items():
        if k not in usec:
            print("  %-6s missing in ours (original vsize 0x%x)" % (k, vs))
            continue
        vb, vsb, rawb = usec[k]
        print("  %-6s va 0x%06x vs 0x%06x  vsize 0x%06x vs 0x%06x (%+d)" % (k, va, vb, vs, vsb, vsb - vs))
    print("functions: %d in both, %d at their original address (%.1f%%)" % (len(both), placed, 100.0 * placed / len(g)))
    print("size queue: %d functions with another padded size; out of order in the link: %d pairs" % (
        len(sizeq), len(misorder)))
    for n in sizeq[:5]:
        print("  first: 0x%08x %s unit=%s %s orig=%d ours=%d" % (g[n][0], n, g[n][1], g[n][2], osz[n], usz[n]))
    for x, y in misorder[:5]:
        print("  order: %s (0x%x) is followed by %s (0x%x)" % (x, g[x][0], y, g[y][0]))


if __name__ == "__main__":
    main()
