"""Flip search: which single change of a function's source makes VC6 emit a wanted instruction pattern?

    python tools/c2dag/flip.py src/d3_int_io.c d3io_AttachStock --want "lea esi, DWORD PTR \\[ecx\\+edx\\]"
    python tools/c2dag/flip.py SRC FUNC --want REGEX [--avoid REGEX] [--try OUT.txt] [--base VARIANTS.txt LABEL]

Every statement line of FUNC is deleted, duplicated, swapped with the next one, and its tokens are replaced
(constants, comparison and logic operators, int types); each mutant is compiled with the project's flags and the
function's listing (one instruction per line, single spaces, as VC6's /FA prints it) is searched for --want (and
must not contain --avoid). The text starts with the function's frame symbols (`_name$ = -OFFSET` lines), so a frame
layout can be wanted too: --want '_cityobj.$ = -1356' (the $ of the symbol escaped or as .). The mutants are NOT meant to be kept: most change the meaning. They show which part
of the function the wanted code depends on - the lever to respell by hand. --try writes the hits as a `dcp try`
variants file (labels m0, m1, ..), so their distance to the original can be measured: a hit at `byte` whose
change keeps the meaning is the answer (di_UpdateButtons: a volatile view against the plain field).
--base applies one variant of a `dcp try` file (whole-function text) first and mutates that.
"""
import argparse, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
VC = os.environ.get("C2DAG_VC", r"C:\Tools\MSVC600-SP5PP")
DX = os.environ.get("C2DAG_DX", r"C:\Tools\DXSDK61\include")
OUT = os.path.join(ROOT, "build", "c2dag", "flip")
TOKENS = ((r"\b0\b", ["1"]), (r"\b1\b", ["0", "2"]), (r"\b2\b", ["3"]), (r"\b3\b", ["2"]), (r"0x[0-9a-fA-F]+", ["0x7"]),
          (r"<=", ["<"]), (r">=", [">"]), (r"==", ["!="]), (r"!=", ["=="]), (r" < ", [" <= "]), (r" > ", [" >= "]),
          (r"&&", ["||"]), (r"\|\|", ["&&"]), (r"\bint\b", ["unsigned", "short", "char"]),
          (r"\bunsigned char\b", ["int"]), (r"\bshort\b", ["int"]), (r"\bfloat\b", ["double"]),
          (r"\+\+", ["--"]), (r" \+ ", [" - "]), (r" - ", [" + "]), (r"\belse\b", ["else if (1)"]),
          (r"\bvolatile\b", [""]), (r"\(\w[\w ]*\*\)", [""]), (r"\bstatic ", ["", "extern "]),
          (r"\bextern ", ["", "static "]), (r"\bconst ", [""]), (r"\bvoid \*", ["int ", "char *"]), (r" = \{0\}", [""]),
          (r"\bunsigned ", [""]), (r"\[2\]", ["[3]"]))


def env():
    e = dict(os.environ)
    e["INCLUDE"] = VC + r"\VC98\Include"
    e["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + e["PATH"]
    return e


def listing(tag, text, fn):
    c, a, o = (os.path.join(OUT, tag + x) for x in (".c", ".asm", ".obj"))
    with open(c, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    r = subprocess.run([VC + r"\VC98\Bin\cl.exe", "/nologo", "/c", "/O2", "/Gy", "/I" + os.path.join(ROOT, "include"),
                        "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fo" + o, "/Fa" + a, c],
                       env=env(), capture_output=True, text=True, cwd=ROOT)
    out = None
    if r.returncode == 0:
        out, on, frame = [], False, []
        for l in open(a, encoding="latin-1"):
            if re.match(r"_\S+\$\S* = -?\d+", l):
                frame.append(re.sub(r"\s+", " ", l.split(";")[0].strip()))  # a frame symbol: _name$ = offset
            elif re.match(r"[_@]%s(@\d+)?\s+PROC" % re.escape(fn), l):
                on = True
                out = frame  # the function's frame symbols come first, so --want can ask for a layout
            elif re.match(r"\S+\s+PROC", l):
                frame = []
            elif re.match(r"[_@]%s(@\d+)?\s+ENDP" % re.escape(fn), l):
                on = False
            elif on and re.match(r"\t[a-z]", l):
                out.append(re.sub(r"\s+", " ", l.split(";")[0].strip()))
    for p in (c, a, o):
        try:
            os.remove(p)
        except OSError:
            pass
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--want", required=True); ap.add_argument("--avoid")
    ap.add_argument("--try", dest="tryfile"); ap.add_argument("--base", nargs=2)
    ap.add_argument("--scope", choices=("func", "file"), default="func",
                    help="file: mutate every line of the unit OUTSIDE the function too (declarations, macros, the "
                         "functions defined above it); --try is then not written")
    a = ap.parse_args()
    os.makedirs(OUT, exist_ok=True)
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    lines = text.split("\n")
    s = next(i for i, l in enumerate(lines) if re.match(r"\w[\w \*]*\b%s\(" % re.escape(a.func), l) and not l.rstrip().endswith(";")
             and i + 1 < len(lines) and lines[i + 1].startswith("{"))  # the definition, not a prototype or a use
    e = next(i for i in range(s, len(lines)) if lines[i] == "}")
    cs = s
    while cs > 0 and not lines[cs - 1].startswith(("}", "#", ";")) and lines[cs - 1].strip():
        cs -= 1  # the address comment above the definition belongs to the function's text
    if a.base:
        vt = open(a.base[0], encoding="utf-8").read().replace("\r\n", "\n")
        m = re.search(r"^### %s\n(.*?)(?=^### |\Z)" % re.escape(a.base[1]), vt, re.S | re.M)
        new = m.group(1).rstrip("\n").split("\n")
        lines = lines[:cs] + new + lines[e + 1:]
        s = next(i for i in range(cs, len(lines)) if re.match(r"\w[\w \*]*\b%s\(" % re.escape(a.func), lines[i]))
        e = next(i for i in range(s, len(lines)) if lines[i] == "}")
    muts = []
    rng = range(s + 2, e) if a.scope == "func" else [i for i in range(len(lines)) if not (cs <= i <= e)]
    for i in rng:
        l = lines[i]
        if not l.strip() or l.strip().startswith(("/*", "*", "//")) or l.strip().endswith("*/"):
            continue
        n = i - s if a.scope == "func" else i + 1
        muts.append(("del %d: %s" % (n, l.strip()[:56]), lines[:i] + lines[i + 1:]))
        muts.append(("dup %d: %s" % (n, l.strip()[:56]), lines[:i] + [l, l] + lines[i + 1:]))
        if i + 1 < len(lines) and (a.scope == "file" or i + 1 < e) and lines[i + 1].strip():
            muts.append(("swap %d: %s" % (n, l.strip()[:56]), lines[:i] + [lines[i + 1], l] + lines[i + 2:]))
        for pat, reps in TOKENS:
            for k, m in enumerate(re.finditer(pat, l)):
                for rep in reps:
                    muts.append(("tok %d#%d %s->%s: %s" % (n, k, m.group(0), rep, l.strip()[:48]),
                                 lines[:i] + [l[:m.start()] + rep + l[m.end():]] + lines[i + 1:]))
    want = re.compile(a.want)
    avoid = re.compile(a.avoid) if a.avoid else None

    def ok(ins):
        t = "\n".join(ins)
        return bool(want.search(t)) and not (avoid and avoid.search(t))

    base = listing("base", "\n".join(lines), a.func)
    if base is None:
        sys.exit("the base does not compile")
    print("base: %d instructions, wanted pattern %s; %d mutants" % (len(base), "present" if ok(base) else "absent", len(muts)), flush=True)
    b0 = ok(base)

    def run(k):
        ins = listing("m%d" % k, "\n".join(muts[k][1]), a.func)
        return None if ins is None else (ok(ins), len(ins))

    with ThreadPoolExecutor(max_workers=8) as ex:
        res = list(ex.map(run, range(len(muts))))
    hits = [(k, r[1]) for k, r in enumerate(res) if r and r[0] != b0]
    print("compiled %d, flips %d" % (sum(1 for r in res if r), len(hits)))
    for k, n in sorted(hits, key=lambda h: abs(h[1] - len(base))):
        print("  m%-4d n=%-5d %s" % (k, n, muts[k][0]))
    if a.tryfile and a.scope == "func":
        with open(a.tryfile, "w", encoding="utf-8", newline="\n") as f:
            for k, n in hits:
                ls = muts[k][1]
                d = len(ls) - len(lines)
                f.write("### m%d\n%s\n\n" % (k, "\n".join(ls[cs:e + 1 + d])))
        print("dcp try variants written to", a.tryfile)


if __name__ == "__main__":
    main()
