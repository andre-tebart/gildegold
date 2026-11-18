"""vsize.py SRC FUNC VARIANTS [LABEL...|all]: the raw code size of FUNC per variant (VC6 /O2 /Gy section size,
trailing int3/nop fill dropped), with the control first. The size queue (tools/exe/exemeasure.py --queue) needs the
padded size (raw rounded up to 16) to equal the original's; dcp try does not print sizes. The raw size includes a
switch's jump table (it sits in the function's section), so it is larger than `dcp grade`'s "size X vs Y" for
functions with a switch; compare against the original's padded size, not dcp grade's figure.
Example: uv run --with capstone python tools/exe/vsize.py src/ch.c ch_ActTurn build/climb/v.txt all"""
import sys, os, re, struct
sys.path.insert(0, r'C:\Users\andre\Documents\gildegold\tools\c2dag')
import vdiff
if len(sys.argv) < 4 or not sys.argv[1].endswith(".c"):
    sys.exit(__doc__)
vdiff.SRC, vdiff.FN = os.path.abspath(sys.argv[1]), sys.argv[2]
vf = sys.argv[3]
labels = sys.argv[4:]
if not labels or labels == ['all']:
    labels = [l[4:].strip() for l in open(vf, encoding='latin-1') if l.startswith('### ')]
def size(o):
    d = open(o, "rb").read()
    mach, nsec, _, symoff, nsym, _, _ = struct.unpack_from("<HHIIIHH", d, 0)
    strtab = symoff + nsym * 18; i = 0; hit = None
    while i < nsym:
        name8, val, sec, typ, cls, naux = struct.unpack_from("<8sIhHBB", d, symoff + i * 18)
        if name8[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", name8, 4)[0]; nm = d[strtab + off:d.index(b"\0", strtab + off)].decode()
        else: nm = name8.rstrip(b"\0").decode()
        if re.match(r"[_@]%s(@\d+)?$" % re.escape(vdiff.FN), nm) and sec > 0: hit = sec
        i += 1 + naux
    so = 20 + (hit - 1) * 40
    rs, rp = struct.unpack_from("<8sIIIIIIHHI", d, so)[3:5]
    code = d[rp:rp+rs]
    n = len(code)
    while n and code[n-1] in (0x90, 0xcc): n -= 1
    return n
print("control", size(vdiff.build(None, None)))
for l in labels:
    try: print(l, size(vdiff.build(vf, l)))
    except SystemExit as e: print(l, 'ERR', str(e)[:200])
