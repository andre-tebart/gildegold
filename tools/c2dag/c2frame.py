r"""How VC6 lays out one function's stack frame, and why (C2 13.00.9044, stack.c).

    python tools/c2dag/c2frame.py src/main.c main_ChooseCity          # the symbol list and the slot table
    python tools/c2dag/c2frame.py src/main.c main_ChooseCity --refs   # plus every counted reference, in walk order
    python tools/c2dag/c2frame.py src/main.c FUNC --variants FILE [--label L]   # one dcp try variant

Rows:
  L  the locals at layout time, in list order: name, size, count
  B  the slot table before the sort: size, weight (sum of the members' counts), dens = weight * 1000 / size, members
  A  the slot table after the sort; index 0 is the lowest address (nearest esp); fixed parameter slots come first
  T / R (--refs)  every tuple with its operands, and each counted reference in the order of the walk

The rule (read from c2.dll and confirmed by experiment):
  * count: one per operand that names the local, over all blocks, NO loop weighting. It is taken after global
    optimisation, register allocation and instruction selection but BEFORE the late passes (tail merging /
    cross-jumping, the late store-to-load peephole). So it is not the listing's count: a statement written in
    both arms of an if (merged late) counts twice; a reload that the late peephole removes counts; a lea that
    is later folded into several memory operands counts once.
  * list: size ascending (the declared size: 0xfc and 0x100 are different groups), then count descending; on a
    tie the local that reached the count FIRST in the walk stays ahead.
  * slots: each local in list order joins the first lifetime-compatible slot (searched from the last slot
    backward), else opens a new one. Scalars and arrays whose address never escaped merge; an array passed to a
    call never merges with later ones.
  * sort: only when the frame is larger than 0x80 bytes: by dens descending, with an UNSTABLE quicksort (equal
    densities come out in an order that depends on the whole table).
  * esp frames are allocated from the last table index down, so the densest slot is nearest esp.
  Declaration order, types and scope never enter.

So a frame hunk is a reference-count question: find the local whose count must be one higher or lower, then the
ordinary source form that has (or has not) that reference at layout time.

How it works: tools/c2dag/c2frm.c is a proxy C2.DLL (as c2proxy.c) with hooks in the packer; it is built into
build/c2dag/frame/bin next to a copy of the compiler and writes to the C2FRM_OUT file.
"""
import argparse, os, re, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
VC = os.environ.get("C2DAG_VC", r"C:\Tools\MSVC600-SP5PP")
DX = os.environ.get("C2DAG_DX", r"C:\Tools\DXSDK61\include")
WORK = os.path.join(ROOT, "build", "c2dag", "frame")
BIN = os.path.join(WORK, "bin")
sys.path.insert(0, HERE)
from c2trace import variant_text  # the same variants reader


def env():
    e = dict(os.environ)
    e["INCLUDE"] = VC + r"\VC98\Include"
    e["LIB"] = VC + r"\VC98\Lib"
    e["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + e["PATH"]
    return e


def setup():
    src = os.path.join(HERE, "c2frm.c")
    dll = os.path.join(BIN, "C2.DLL")
    if os.path.exists(dll) and os.path.getmtime(dll) >= os.path.getmtime(src):
        return
    os.makedirs(BIN, exist_ok=True)
    vb = VC + r"\VC98\Bin"
    for f in os.listdir(vb):
        if re.match(r"(?i)(cl\.exe|c1\.dll|.*\.err|cl\.cfg)$", f):
            shutil.copy2(os.path.join(vb, f), os.path.join(BIN, f))
    shutil.copy2(os.path.join(vb, "C2.DLL"), os.path.join(BIN, "c2real.dll"))
    r = subprocess.run([vb + r"\cl.exe", "/nologo", "/LD", "/O1", src, "/Fe" + os.path.join(WORK, "c2.dll"),
                        "/Fo" + os.path.join(WORK, "c2frm.obj"), "/link", "/BASE:0x20000000", "user32.lib"],
                       env=env(), cwd=WORK, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("building the frame proxy failed:\n" + r.stdout)
    shutil.copy2(os.path.join(WORK, "c2.dll"), dll)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--refs", action="store_true", help="also the walk: T rows (tuples) and R rows (counted references)")
    ap.add_argument("--variants", help="a dcp try variants file: lay out one of its variants instead of the tree's text")
    ap.add_argument("--label", help="the variant (default: the first)")
    a = ap.parse_args()
    setup()
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    tag = os.path.splitext(os.path.basename(a.src))[0]
    if a.variants:
        text, lab = variant_text(text, a.func, a.variants, a.label)
        tag += "_" + re.sub(r"\W", "_", lab)
    c = os.path.join(WORK, tag + ".c")
    frm = os.path.join(WORK, tag + ".frm")
    with open(c, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    e = env()
    e["C2FRM_OUT"] = frm
    e["C2FRM_REFS"] = "1"
    e["C2FRM_TUP"] = "1"
    r = subprocess.run([os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/I" + os.path.join(ROOT, "include"),
                        "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fa" + os.path.join(WORK, tag + ".asm"),
                        "/Fo" + os.path.join(WORK, tag + ".obj"), c], env=e, cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("compile failed:\n" + "\n".join(l for l in r.stdout.splitlines() if "error" in l))
    for b in open(frm, encoding="latin-1").read().split("WALK\n")[1:]:
        if re.search(r"(?m)^FUNC [_@]?%s(@\d+)? " % re.escape(a.func), b):
            print("\n".join(l for l in b.splitlines() if a.refs or not l.startswith(("R ", "T "))))
            return
    raise SystemExit("function not found in the trace (its frame was not packed: an unoptimised or frameless function)")


if __name__ == "__main__":
    main()
