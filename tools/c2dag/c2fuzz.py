"""Call-skip map of VC6's back end (c2.dll): patch one `call rel32` at a time to `xor eax,eax`, compile a probe
file with the patched back end, and record which skipped calls leave a valid but different listing. Those are the
passes (scheduler, cross-jumping, peepholes ..) one can switch off to see the code before them."""
import os, re, shutil, subprocess, sys, ctypes, difflib, threading, queue
import capstone, pefile

HERE = os.path.dirname(os.path.abspath(__file__))
VC = r"C:\Tools\MSVC600-SP5PP"
BIN = VC + r"\VC98\Bin"
WORK = os.path.join(HERE, "c2fuzz")
PROBE = os.environ.get("C2FUZZ_PROBE", os.path.join(HERE, "fz.c"))
ONLY = os.environ.get("C2FUZZ_SITES")
OUTTSV = os.environ.get("C2FUZZ_OUT", os.path.join(HERE, "c2fuzz.tsv"))
ROOT = r"C:\Users\andre\Documents\gildegold"
NW = 8
ENV = dict(os.environ)
ENV["INCLUDE"] = VC + r"\VC98\Include"
ENV["PATH"] = VC + r"\Common\MSDev98\Bin;" + ENV["PATH"]
ctypes.windll.kernel32.SetErrorMode(0x8003)  # no crash dialogs, inherited by the children


def call_sites(data):
    pe = pefile.PE(data=data)
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    base = pe.OPTIONAL_HEADER.ImageBase
    va0 = base + text.VirtualAddress
    raw0 = text.PointerToRawData
    code = data[raw0:raw0 + text.SizeOfRawData]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    sites, off = [], 0
    n = min(len(code), text.Misc_VirtualSize)
    while off < n:
        got = False
        for ins in md.disasm(code[off:off + 16], va0 + off, 1):
            got = True
            if ins.mnemonic == "call" and ins.size == 5 and code[off] == 0xE8:
                tgt = int(ins.op_str, 16)
                if va0 <= tgt < va0 + n:
                    sites.append((va0 + off, raw0 + off, tgt))
            off += ins.size
        if not got:
            off += 1
    return sites


def norm(asm_path):
    out, fn = {}, None
    for l in open(asm_path, encoding="latin-1"):
        m = re.match(r"(_\w+)\s+PROC", l)
        if m:
            fn = m.group(1); out[fn] = []
        elif re.match(r"_\w+\s+ENDP", l):
            fn = None
        elif fn and re.match(r"\t[a-z]|\$\w+:", l):
            out[fn].append(re.sub(r"\s+", " ", l.split(";")[0].strip()))
    return out


def compile_in(wd, timeout=30):
    asm = os.path.join(wd, "fz.asm")
    if os.path.exists(asm):
        os.remove(asm)
    try:
        r = subprocess.run([os.path.join(wd, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/I" + ROOT + r"\include", "/I" + ROOT + r"\src", r"/IC:\Tools\DXSDK61\include", "/Fa" + asm,
                            "/Fo" + os.path.join(wd, "fz.obj"), PROBE], env=ENV, cwd=ROOT, capture_output=True,
                           text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return "timeout", None
    if r.returncode != 0 or not os.path.exists(asm):
        return "fail", None
    try:
        return "ok", norm(asm)
    except Exception:
        return "fail", None


def main():
    data = open(os.path.join(BIN, "C2.DLL"), "rb").read()
    sites = call_sites(data)
    starts = sorted(set(t for _, _, t in sites))
    print("call sites", len(sites), "targets", len(starts), flush=True)
    os.makedirs(WORK, exist_ok=True)
    dirs = []
    for w in range(NW):
        d = os.path.join(WORK, "w%d" % w)
        os.makedirs(d, exist_ok=True)
        for f in os.listdir(BIN):
            if re.match(r"(?i)(cl\.exe|c1\.dll|c2\.dll|.*\.err|cl\.cfg|msvcr.*|mspdb.*|msobj.*)$", f):
                shutil.copy2(os.path.join(BIN, f), os.path.join(d, f))
        dirs.append(d)
    st, base = compile_in(dirs[0])
    assert st == "ok", st
    print("baseline functions", {k: len(v) for k, v in base.items()}, flush=True)
    import bisect
    lim = int(sys.argv[1]) if len(sys.argv) > 1 else len(sites)
    q = queue.Queue()
    if ONLY:
        want = set(int(x, 16) for x in open(ONLY).read().split())
        sites = [s for s in sites if s[0] in want]
    for s in sites[:lim]:
        q.put(s)
    res, lock = [], threading.Lock()

    def worker(d):
        while True:
            try:
                va, raw, tgt = q.get_nowait()
            except queue.Empty:
                return
            b = bytearray(data)
            b[raw:raw + 5] = b"\x33\xc0\x90\x90\x90"
            with open(os.path.join(d, "C2.DLL"), "wb") as f:
                f.write(b)
            st, out = compile_in(d)
            desc = ""
            watch = os.environ.get("C2FUZZ_WATCH")
            if st == "ok" and watch:
                fnw, x, y = watch.split("|")
                L = out.get(fnw, [])
                ix = next((i for i, l in enumerate(L) if x in l), -1)
                iy = next((i for i, l in enumerate(L) if i > ix - 3 and y in l), -1) if ix >= 0 else -1
                wdesc = "W:%s" % ("x<y" if 0 <= ix < iy else "y<x" if 0 <= iy < ix else "?")
            else:
                wdesc = ""
            if st == "ok":
                if out == base:
                    st = "same"
                else:
                    st = "diff"
                    parts = []
                    for k in base:
                        a, c = base[k], out.get(k)
                        if c is None:
                            parts.append("%s:missing" % k)
                        elif a != c:
                            same_set = sorted(a) == sorted(c)
                            parts.append("%s:%s%d/%d" % (k, "reorder" if same_set else "", len(c), len(a)))
                    desc = " ".join(parts)
            fn = starts[bisect.bisect_right(starts, va) - 1] if starts[0] <= va else 0
            with lock:
                res.append((va, tgt, fn, st, (wdesc + " " + desc).strip()))
                if len(res) % 500 == 0:
                    print(len(res), flush=True)

    ts = [threading.Thread(target=worker, args=(d,)) for d in dirs]
    [t.start() for t in ts]
    [t.join() for t in ts]
    res.sort()
    from collections import Counter
    print(Counter(r[3] for r in res))
    with open(OUTTSV, "w") as f:
        for va, tgt, fn, st, desc in res:
            f.write("0x%x\t0x%x\t0x%x\t%s\t%s\n" % (va, tgt, fn, st, desc))


main()
