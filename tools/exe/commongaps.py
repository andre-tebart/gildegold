"""commongaps: the original's common area checked against LINK's common alignment, name order aside.

    python tools/exe/commongaps.py [--unit U] [--all] [--perm N]

Reads the data maps datameasure caches (build/exe/datamap, `datameasure.py --refresh` first). For every pair of
neighbouring commons in the ORIGINAL's address order it compares the gap between them with the pad LINK puts before
the second one (LINK 6.00's common rule, notes/vc6-data.md: each common aligned to the next power of two of its
size, at most 32). A gap larger than that pad is a common the tree does not declare there (a missing tentative
definition, or the first one is declared too small); a negative gap is a size too big. Neither depends on the
in-group name order, so a group whose extent is off (`datameasure.py --groups`) is either a gap / overlap listed
here or the name order alone.

Per group (our unit) it also prints the original's extent and the range of extents our members can take over N
random orders (--perm, default 2000): an original extent outside that range is a size or member error, inside it
the name order can explain it.

Why (data layout s12): cm's split groups (cmscene -64, cm +32, amt -16) had to be told apart from name-order pads.
"""
import argparse, glob, os, random, re

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
CACHE = os.path.join(ROOT, "build", "exe", "datamap")
COMMON_START = 0x6f3300


def align(size):
    a = 1
    while a < size and a < 32:
        a *= 2
    return a


def extent(sizes):
    p = 0
    first = None
    for s in sizes:
        al = align(s)
        p = (p + al - 1) // al * al
        if first is None:
            first = p
        p += s
    return p - first


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--unit")
    ap.add_argument("--all", action="store_true", help="print every pair, not only gaps / overlaps")
    ap.add_argument("--perm", type=int, default=2000)
    a = ap.parse_args()
    decl = {}
    for f in glob.glob(os.path.join(CACHE, "*.txt")):
        u = os.path.basename(f)[:-4]
        for l in open(f, encoding="utf-8", errors="replace"):
            m = re.search(r"at=0x([0-9a-f]+)\s+name=(\S+).*?section=(\S+)\s+size=((?:0x)?[0-9a-f]+)", l)
            if m and int(m.group(1), 16) >= COMMON_START:
                decl[m.group(2)] = (int(m.group(1), 16), int(m.group(4), 0), u)
    rows = sorted((v[0], k, v[1], v[2]) for k, v in decl.items())
    print("pairs (original order): gap = next start - this end; pad = LINK's alignment pad before next")
    for (a0, k0, s0, u0), (a1, k1, s1, u1) in zip(rows, rows[1:]):
        end = a0 + s0
        al = align(s1)
        pad = (al - end % al) % al
        gap = a1 - end
        bad = gap != pad
        if (a.unit and a.unit not in (u0, u1)):
            continue
        if bad or a.all:
            what = "" if not bad else ("  OVERLAP" if gap < 0 else "  +%d unexplained" % (gap - pad))
            print("  0x%08x %-26s %-10s size %-7d -> 0x%08x %-26s %-10s gap %6d pad %2d%s" % (
                a0, k0, u0, s0, a1, k1, u1, gap, pad, what))
    g = {}
    for at, k, s, u in rows:
        g.setdefault(u, []).append((at, k, s))
    print("groups: original extent vs the range our members can take (random orders)")
    for u, ms in sorted(g.items(), key=lambda kv: kv[1][0][0]):
        if a.unit and u != a.unit:
            continue
        o = ms[-1][0] + ms[-1][2] - ms[0][0]
        sz = [s for _, _, s in ms]
        lo = hi = extent(sz)
        for _ in range(a.perm if len(sz) > 1 else 0):
            random.shuffle(sz)
            e = extent(sz)
            lo, hi = min(lo, e), max(hi, e)
        flag = "" if lo <= o <= hi else "  OUTSIDE"
        print("  0x%08x %-12s %3d commons  original %8d  ours %d..%d%s" % (ms[0][0], u, len(ms), o, lo, hi, flag))


if __name__ == "__main__":
    main()
