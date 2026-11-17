"""hunksz.py ASMDIFF ORIG_DISASM OURS_DISASM: per hunk of an asmdiff, the byte length on each side, so a size gap
is located before any variant (which hunk is short, by how much).
  dcp asmdiff NAME > a.txt; dcp disasm NAME > o.txt; dcp disasm --ours NAME > u.txt; python tools/exe/hunksz.py a.txt o.txt u.txt"""
import re, sys
def sizes(f):
    d = {}
    for l in open(f):
        m = re.match(r"\s+0x([0-9a-f]{8})\s+([0-9a-f]+)\s", l)
        if m: d[int(m.group(1),16)] = len(m.group(2))//2
    return d
o = sizes(sys.argv[2]); u = sizes(sys.argv[3])
cur = None; mi = pl = 0; first = None
def flush():
    global mi, pl, first
    if mi != pl: print("hunk at ours %s: ours %d orig %d (%+d)" % (first, mi, pl, pl - mi))
    mi = pl = 0; first = None
for l in open(sys.argv[1]):
    m = re.match(r"([-+~ ])  0x([0-9a-f]{8})", l)
    if not m: continue
    k, a = m.group(1), int(m.group(2), 16)
    if k in ' ~': flush(); continue
    if first is None: first = hex(a)
    if k == '-': mi += u.get(a, 0)
    else: pl += o.get(a, 0)
flush()
