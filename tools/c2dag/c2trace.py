"""The register allocator's decisions for one function, step by step (C2 13.00.9044).

    python tools/c2dag/c2trace.py src/main.c main_ChooseHistory            # the assignment sequence
    python tools/c2dag/c2trace.py src/main.c main_ChooseHistory --raw       # every trace line
    python tools/c2dag/c2trace.py src/main.c FUNC --variants FILE [--label L]  # dcp try variants (whole text or blocks)

`c2dag.py --alloc` prints the candidates after the FIRST priority pass and the register each one ended with. That
is not enough to predict an outcome: assigning a register can split other candidates, and the pieces re-enter the
queue with new priorities. This tool prints the order in which the allocator really pops candidates, each with
its priority, savings, the registers still available to it, the score per register and the one it takes.

The rule (read from c2.dll, color.c; confirmed on main_ChooseHistory):
  * queue: descending priority; equal priority: the candidate created later first. Split pieces are re-queued.
  * avail: a candidate live across a call keeps ebx / ebp / esi / edi only; one referenced by a byte-sized
    operand keeps eax / ecx / edx / ebx only (across a call: ebx alone); a register given to an interfering
    candidate is removed from the others.
  * choice: score[r] = sum over the unprocessed interfering neighbours with savings > 0 of their preference
    counts for r, plus 100 * savings for a neighbour whose ONLY register is r, minus the candidate's own
    preference for r (copies to / from a physical register, e.g. eax of a call result). Lowest score wins, ties in
    the order eax, ecx, edx, esi, edi, ebx, ebp.
  * savings >= 1 is needed for a register. Under /O2 a use counts 1 << loop depth, a constant only where the
    other operand is memory, and each load of a constant costs the same weight.
  * pressure: when the registers run out at an instruction (a call once all four callee-saved ones are taken),
    every other candidate live across it is split there.
  * a block where the candidate is dead (a definition follows on every path) costs it nothing.

How it works: tools/c2dag/c2trace.c is the c2dag proxy with more hooks (the queue insert, the choice, the
assignment, the splits); it is built into build/c2dag/trace/bin next to a copy of the compiler. C2TRACE_FUNC
selects the function, the `T ...` lines go to the C2DAG_OUT file.
"""
import argparse, os, re, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
VC = os.environ.get("C2DAG_VC", r"C:\Tools\MSVC600-SP5PP")
DX = os.environ.get("C2DAG_DX", r"C:\Tools\DXSDK61\include")
WORK = os.path.join(ROOT, "build", "c2dag", "trace")
BIN = os.path.join(WORK, "bin")


def env():
    e = dict(os.environ)
    e["INCLUDE"] = VC + r"\VC98\Include"
    e["LIB"] = VC + r"\VC98\Lib"
    e["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + e["PATH"]
    return e


def setup():
    src = os.path.join(HERE, "c2trace.c")
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
                        "/Fo" + os.path.join(WORK, "c2trace.obj"), "/link", "/BASE:0x20000000", "user32.lib"],
                       env=env(), cwd=WORK, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("building the trace proxy failed:\n" + r.stdout)
    shutil.copy2(os.path.join(WORK, "c2.dll"), dll)


def function_span(lines, func):
    s = next(i for i, l in enumerate(lines) if re.match(r"\w[\w \*]*\b%s\(" % re.escape(func), l) and not l.rstrip().endswith(";")
             and i + 1 < len(lines) and lines[i + 1].startswith("{"))
    e = next(i for i in range(s, len(lines)) if lines[i] == "}")
    cs = s
    while cs > 0 and not lines[cs - 1].startswith(("}", "#", ";")) and lines[cs - 1].strip():
        cs -= 1
    return cs, s, e


def variant_text(text, func, vfile, label):
    lines = text.split("\n")
    cs, s, e = function_span(lines, func)
    vt = open(vfile, encoding="latin-1").read().replace("\r\n", "\n")
    labels = re.findall(r"^### (.+)$", vt, re.M)
    label = label or labels[0]
    m = re.search(r"^### %s\n(.*?)(?=^### |\Z)" % re.escape(label), vt, re.S | re.M)
    if not m:
        raise SystemExit("no variant %r (have: %s)" % (label, ", ".join(labels)))
    body = m.group(1)
    fn = "\n".join(lines[cs:e + 1])
    if "<<<" in body:
        for b in re.finditer(r"<<<\n(.*?)\n?===\n(.*?)\n?>>>", body, re.S):
            if fn.count(b.group(1)) != 1:
                raise SystemExit("old text occurs %d times: %r" % (fn.count(b.group(1)), b.group(1)[:60]))
            fn = fn.replace(b.group(1), b.group(2))
    else:
        fn = body.strip("\n")
    return "\n".join(lines[:cs]) + "\n" + fn + "\n" + "\n".join(lines[e + 1:]), label


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--variants", help="a dcp try variants file: trace one of its variants instead of the tree's text")
    ap.add_argument("--label", help="the variant (default: the first)")
    ap.add_argument("--raw", action="store_true", help="every T line of the trace")
    ap.add_argument("--allpass", action="store_true", help="also the re-prioritisation passes after splits")
    a = ap.parse_args()
    setup()
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    tag = os.path.splitext(os.path.basename(a.src))[0]
    if a.variants:
        text, lab = variant_text(text, a.func, a.variants, a.label)
        tag += "_" + re.sub(r"\W", "_", lab)
    c = os.path.join(WORK, tag + ".c")
    dag = os.path.join(WORK, tag + ".dag")
    with open(c, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    if os.path.exists(dag):
        os.remove(dag)
    e = env()
    e["C2DAG_OUT"] = dag
    e["C2TRACE_FUNC"] = a.func
    e["C2TRACE_NOXJ"] = "1"     # without the scheduling-graph dump (c2dag.py has it)
    if a.allpass:
        e["C2TRACE_ALLPASS"] = "1"
    r = subprocess.run([os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/I" + os.path.join(ROOT, "include"),
                        "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fa" + os.path.join(WORK, tag + ".asm"),
                        "/Fo" + os.path.join(WORK, tag + ".obj"), c], env=e, cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("compile failed:\n" + "\n".join(l for l in r.stdout.splitlines() if "error" in l))
    T = [l.rstrip() for l in open(dag, encoding="latin-1") if l.startswith("T ")]
    if not T:
        raise SystemExit("no trace lines: is %s the function's name in the listing?" % a.func)
    if a.raw:
        print("\n".join(T))
        return
    # the summary: the lines that queue, choose, assign and split (the kinds the proxy writes; see c2trace.c)
    keep = re.compile(r"T (QUEUE|ASSIGN|SPLIT|UNPROFITABLE|NOREG)\b")
    shown = [l for l in T if keep.match(l)]
    print("\n".join(shown if shown else T))
    print("(%d trace lines in %s; --raw prints all)" % (len(T), dag))


if __name__ == "__main__":
    main()
