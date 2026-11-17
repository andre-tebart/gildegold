"""vlist.py SRC FUNC [VARIANTS LABEL]: VC6 /FAs listing of FUNC (source lines interleaved) for the tree text or one
variant of a dcp try file, from a scratch compile (nothing in the tree is written; work files in build/c2dag/vlist).
Read a size or schedule gap per source line: which statement VC6 emitted where."""
import sys, os, re, subprocess
sys.path.insert(0, r'C:\Users\andre\Documents\gildegold\tools\c2dag')
import c2trace
ROOT = r'C:\Users\andre\Documents\gildegold'
VC = r"C:\Tools\MSVC600-SP5PP"; DX = r"C:\Tools\DXSDK61\include"
SRC, FN = os.path.abspath(sys.argv[1]), sys.argv[2]
text = open(SRC, encoding="latin-1", newline="").read().replace("\r\n", "\n")
if len(sys.argv) > 4:
    text, _ = c2trace.variant_text(text, FN, sys.argv[3], sys.argv[4], src=SRC)
H = os.path.join(ROOT, 'build', 'c2dag', 'vlist')
os.makedirs(H, exist_ok=True)
c = os.path.join(H, 'w.c'); o = c[:-2] + '.obj'; a = c[:-2] + '.asm'
open(c, "w", encoding="latin-1", newline="\n").write(text)
e = dict(os.environ); e["INCLUDE"] = VC + r"\VC98\Include"
e["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + e["PATH"]
r = subprocess.run([VC + r"\VC98\Bin\cl.exe", "/nologo", "/c", "/O2", "/Gy", "/FAs", "/Fa" + a, "/I" + os.path.join(ROOT, "include"),
                    "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fo" + o, c], env=e, capture_output=True, text=True, cwd=ROOT)
if r.returncode: raise SystemExit(r.stdout + r.stderr)
L = open(a, encoding='latin-1').read().split('\n')
on = False
for l in L:
    if re.match(r'_%s\s+PROC' % FN, l): on = True
    if on: print(l)
    if on and re.match(r'_%s\s+ENDP' % FN, l): break
