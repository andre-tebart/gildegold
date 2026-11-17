"""holes: the gaps between our declared data in an original address range, with an alignment verdict and readers.

    uv run python tools/exe/holes.py LO HI [--section bss|data|rdata] [--all]

Reads the `dcp data map` cache (build/exe/datamap, refresh with `tools/exe/datameasure.py --refresh`), sorts the
declared data of SECTION (default bss) in [LO, HI) by original address and prints each gap between the end of one
datum and the start of the next:

    0x00631074..0x00631078     4 real=  4 DATA  after ch_PathOffset [ch]  before sv_PanelCartChar [ch]  -
    0x0063172c..0x00631730     4 real=  0 PAD   after qnt_Colors [qnt]  before qnt_Closest [qnt]  -

`real` is the gap after 4-aligning the previous datum's end. PAD: the gap is the 8-alignment before a datum of
>= 64 bytes (VC6 8-aligns those, also in .bss / .data) or before the first datum of a unit whose section holds one;
a PAD needs no declaration, our link reproduces it. The last column lists the original functions whose instruction
operands point into the gap (from the build/orig-asm cache of tools/search/grep_orig.py; run it once) and the image
offsets of dwords pointing into it (`-` = no reader at all: data of functions /OPT:REF removed, the unread blocks of
s8 / s9 such as d2_UnreadBss). Gaps after a byte up to the next 4-aligned address are not printed (--all prints them).
"""
import argparse, glob, os, re, struct

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
EXE = None


def image():
    toml = open(os.path.join(ROOT, "decompanion.toml"), encoding="utf-8").read()
    game = re.search(r'^game\s*=\s*"([^"]+)"', toml, re.M).group(1)
    return open(os.path.join(game, "GildeGold.exe"), "rb").read()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("lo"); ap.add_argument("hi")
    ap.add_argument("--section", default="bss")
    ap.add_argument("--all", action="store_true")
    a = ap.parse_args()
    lo, hi = int(a.lo, 16), int(a.hi, 16)
    syms, big = [], {}
    for f in glob.glob(os.path.join(ROOT, "build", "exe", "datamap", "*.txt")):
        u = os.path.basename(f)[:-4]
        for l in open(f, encoding="utf-8", errors="replace"):
            m = re.search(r"at=0x([0-9a-f]+)\s+name=(\S+).*?section=(\S+)\s+size=((?:0x)?[0-9a-f]+)", l)
            if not m or m.group(3) != a.section:
                continue
            ad = int(m.group(1), 16)
            sz = int(m.group(4), 16) if m.group(4).startswith("0x") else int(m.group(4))
            if sz >= 64:
                big[u] = True
            if lo <= ad < hi:
                syms.append((ad, sz, m.group(2), u))
    syms.sort()
    refs = {}
    for f in glob.glob(os.path.join(ROOT, "build", "orig-asm", "*.asm")):
        n = os.path.basename(f)[:-4]
        for m in re.finditer(r"0x([0-9a-f]{6,8})\b", open(f, encoding="utf-8", errors="replace").read()):
            v = int(m.group(1), 16)
            if lo <= v < hi:
                refs.setdefault(v, set()).add(n)
    b = image()
    dws = {}
    for off in range(0x400, len(b) - 3):
        v = struct.unpack_from("<I", b, off)[0]
        if lo <= v < hi:
            dws.setdefault(v, []).append(off)
    end, prev = lo, None
    for ad, sz, n, u in syms:
        if ad > end and prev:
            al = (end + 3) & ~3
            need8 = sz >= 64 or (u != prev[3] and big.get(u))
            pad = need8 and ad % 8 == 0 and ((end + 7) & ~7) == ad
            if ad - al > 0 or a.all:
                rd = sorted({x for v in range(end, ad) for x in refs.get(v, ())})
                pt = [hex(0x400000 + o) for v in range(end, ad) for o in dws.get(v, [])]
                who = " ".join(rd + pt) or "-"
                print("0x%08x..0x%08x %5d real=%3d %-5s after %s [%s]  before %s [%s]  %s" % (
                    end, ad, ad - end, 0 if pad else max(ad - al, 0), "PAD" if pad else "DATA", prev[2], prev[3], n, u, who))
        end = max(end, ad + sz)
        prev = (ad, sz, n, u)


if __name__ == "__main__":
    main()
