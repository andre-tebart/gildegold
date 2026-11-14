"""Compile a file with a c2.dll in which chosen call sites are skipped, and print one function's listing
(or a unified diff against the unpatched back end).

  c2pass.py SRC FUNC [--skip 0xVA[,0xVA..]] [--diff] [--blocks] [--flags "/I.. .."]
"""
import os, re, shutil, subprocess, sys, difflib, argparse, hashlib
import pefile

HERE = os.path.dirname(os.path.abspath(__file__))
VC = r"C:\Tools\MSVC600-SP5PP"
BIN = VC + r"\VC98\Bin"
ROOT = r"C:\Users\andre\Documents\gildegold"
ENV = dict(os.environ)
ENV["INCLUDE"] = VC + r"\VC98\Include"
ENV["PATH"] = VC + r"\Common\MSDev98\Bin;" + ENV["PATH"]


def bindir(skips):
    key = hashlib.md5(",".join("%x" % s for s in sorted(skips)).encode()).hexdigest()[:10] if skips else "plain"
    d = os.path.normpath(os.path.join(HERE, "..", "..", "build", "c2dag", "c2pass", key))  # under /build/ (ignored): compiler copies never land in the tree
    if not os.path.exists(os.path.join(d, "C2.DLL")):
        os.makedirs(d, exist_ok=True)
        for f in os.listdir(BIN):
            if re.match(r"(?i)(cl\.exe|c1\.dll|c2\.dll|.*\.err|cl\.cfg|msvcr.*|mspdb.*|msobj.*)$", f):
                shutil.copy2(os.path.join(BIN, f), os.path.join(d, f))
        data = bytearray(open(os.path.join(BIN, "C2.DLL"), "rb").read())
        pe = pefile.PE(data=bytes(data))
        for va in skips:
            off = pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)
            assert data[off] == 0xE8, "0x%x is not a call rel32" % va
            data[off:off + 5] = b"\x33\xc0\x90\x90\x90"
        open(os.path.join(d, "C2.DLL"), "wb").write(data)
    return d


def listing(src, fn, skips, blocks=False, flags=()):
    d = bindir(skips)
    tag = os.path.splitext(os.path.basename(src))[0]
    asm = os.path.join(d, tag + ".asm")
    cmd = [os.path.join(d, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy"] + (["/d2FAF"] if blocks else []) + list(flags) + \
          ["/I" + ROOT + r"\include", "/I" + ROOT + r"\src", r"/IC:\Tools\DXSDK61\include", "/Fa" + asm,
           "/Fo" + os.path.join(d, tag + ".obj"), os.path.abspath(src)]
    r = subprocess.run(cmd, env=ENV, cwd=ROOT, capture_output=True, text=True, timeout=120)
    if r.returncode:
        raise SystemExit("compile failed: " + "\n".join(l for l in r.stdout.splitlines() if "error" in l)[:400])
    out, on = [], False
    for l in open(asm, encoding="latin-1"):
        if re.match(r"[_@]%s(@\d+)?\s+PROC" % re.escape(fn), l):
            on = True
        elif re.match(r"[_@]%s(@\d+)?\s+ENDP" % re.escape(fn), l):
            on = False
        elif on:
            if re.match(r"\t[a-z]|\$\w+(\$\d+)?:", l):
                out.append(re.sub(r"\s+", " ", l.split(";")[0].strip()))
            elif blocks and l.startswith("; Block("):
                out.append(l.strip())
    return out


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--skip", default="")
    ap.add_argument("--diff", action="store_true")
    ap.add_argument("--blocks", action="store_true")
    ap.add_argument("--flags", default="")
    a = ap.parse_args()
    skips = [int(x, 16) for x in a.skip.split(",") if x]
    fl = a.flags.split()
    new = listing(a.src, a.func, skips, a.blocks, fl)
    if a.diff:
        old = listing(a.src, a.func, [], a.blocks, fl)
        for l in difflib.unified_diff(old, new, "c2", "c2 --skip " + a.skip, lineterm="", n=3):
            print(l)
    else:
        print("\n".join(new))
