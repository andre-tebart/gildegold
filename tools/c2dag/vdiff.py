"""The instruction diff of ONE dcp try variant against the original (dcp try prints only the distance).

    uv run --with capstone python tools/c2dag/vdiff.py src/main.c main_RoundReport VARIANTS.txt LABEL
    uv run --with capstone python tools/c2dag/vdiff.py src/main.c main_RoundReport          # the tree's text

Compiles the unit with the variant applied (a scratch copy under build/c2dag/vdiff, nothing in the tree is
written), disassembles the function from the object file and diffs it against `dcp asmdiff --side original`
with addresses and relocated operands normalised: '-' ours, '+' the original (with its address). Use it to read
what a variant still gets wrong without applying it, building and reverting.
"""
import sys, os, re, struct, subprocess, difflib
HERE0 = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE0))
sys.path.insert(0, HERE0)
import c2trace, capstone
HERE = os.path.join(ROOT, "build", "c2dag", "vdiff")
os.makedirs(HERE, exist_ok=True)
VC = os.environ.get("C2DAG_VC", r"C:\Tools\MSVC600-SP5PP"); DX = os.environ.get("C2DAG_DX", r"C:\Tools\DXSDK61\include")
SRC = FN = None
def build(vfile, label):
    text = open(SRC, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    if vfile:
        text, _ = c2trace.variant_text(text, FN, vfile, label)
    tag = re.sub(r"\W", "_", FN + "_" + (label or "control"))
    c = os.path.join(HERE, "w_" + tag + ".c"); o = c[:-2] + ".obj"
    open(c, "w", encoding="latin-1", newline="\n").write(text)
    e = dict(os.environ); e["INCLUDE"] = VC + r"\VC98\Include"
    e["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + e["PATH"]
    r = subprocess.run([VC + r"\VC98\Bin\cl.exe", "/nologo", "/c", "/O2", "/Gy", "/I" + os.path.join(ROOT, "include"),
                        "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fo" + o, c], env=e, capture_output=True, text=True, cwd=ROOT)
    if r.returncode: raise SystemExit(r.stdout + r.stderr)
    return o
def func(o):
    d = open(o, "rb").read()
    mach, nsec, _, symoff, nsym, _, _ = struct.unpack_from("<HHIIIHH", d, 0)
    strtab = symoff + nsym * 18
    i = 0; hit = None
    while i < nsym:
        name8, val, sec, typ, cls, naux = struct.unpack_from("<8sIhHBB", d, symoff + i * 18)
        if name8[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", name8, 4)[0]; nm = d[strtab + off:d.index(b"\0", strtab + off)].decode()
        else: nm = name8.rstrip(b"\0").decode()
        if re.match(r"[_@]%s(@\d+)?$" % re.escape(FN), nm) and sec > 0: hit = (sec, val)
        i += 1 + naux
    sec, val = hit
    so = 20 + (sec - 1) * 40
    _, vs, va, rs, rp, relp, _, nrel, _, ch = struct.unpack_from("<8sIIIIIIHHI", d, so)
    code = bytearray(d[rp:rp + rs])
    rel = set()
    for k in range(nrel):
        ra, si, rt = struct.unpack_from("<IIH", d, relp + k * 10); rel.add(ra)
        if rt == 6: code[ra:ra+4] = struct.pack("<I", 0x12345678)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = []
    for ins in md.disasm(bytes(code), 0):
        s = ins.mnemonic + " " + ins.op_str
        hasrel = any(a in rel for a in range(ins.address, ins.address + ins.size))
        if ins.mnemonic in ("call",) or ins.mnemonic.startswith("j"):
            s = ins.mnemonic + " X"
        elif hasrel:
            s = re.sub(r"0x[0-9a-f]+|\b\d+\b(?!\])", "X", s) if False else s
            # replace the displacement / immediate carrying the reloc: any number not a small scale
            s = re.sub(r"(?<![\w*])(0x[0-9a-f]+|0)(?![\w])", "X", s)
            s = re.sub(r"\[(.*?) [+-] X\]", r"[\1 + X]", s)
        s = re.sub(r"^(fadd|fsub|fmul) st\((\d)\)", r" st, st()", s)
        s = s.replace("rep stosd dword ptr es:[edi], eax", "rep stosd")
        if s.strip() != "nop": out.append(s.strip())
    return out
def orig():
    r = subprocess.run(["dcp", "asmdiff", FN, "--side", "original"], capture_output=True, text=True, cwd=ROOT)
    out = []
    for l in r.stdout.splitlines():
        m = re.match(r"\s+0x([0-9a-f]{8})  (.*)", l)
        if m:
            s = m.group(2)
            s = re.sub(r"^(call|j\w+) .*", r"\1 X", s)
            out.append((m.group(1), s))
    return out
if __name__ == "__main__":
    if len(sys.argv) < 3:
        raise SystemExit(__doc__)
    SRC, FN = os.path.abspath(sys.argv[1]), sys.argv[2]
    vfile = sys.argv[3] if len(sys.argv) > 3 and sys.argv[3] != "-" else None
    label = sys.argv[4] if len(sys.argv) > 4 else None
    ours = func(build(vfile, label))
    og = orig()
    def norm(s):
        s = re.sub(r"0x[0-9a-f]{5,}", "X", s)
        if s.startswith("push X"): s = "push X"
        s = re.sub(r"\[(.*?) [+-] X\]", r"[\1 + X]", s)
        return s
    a = [norm(x) for x in ours]; b = [norm(x) for _, x in og]
    n = 0
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            for k in range(max(i1, i2 - 2), i2): pass
            print("   ... %d equal (orig %s..)" % (i2 - i1, og[j1][0]))
        else:
            for k in range(i1, i2): print("-  " + a[k]); n += 1
            for k in range(j1, j2): print("+  %s %s" % (og[k][0], b[k])); n += 1
    print("lines differing:", n, "ours", len(a), "orig", len(b))
