"""VC6's scheduling graph for one function: what the back end (C2 13.00.9044, dag.c) sees before it orders the
instructions of each basic block.

    python tools/c2dag/c2dag.py src/d3_snd.c d3sndw_Update            # every block of the function
    python tools/c2dag/c2dag.py src/d3_snd.c d3sndw_Update --ops e,2d  # only blocks with a call and an add
    python tools/c2dag/c2dag.py src/d3_snd.c d3sndw_Update --blocks    # VC6's own block listing (/d2FAF)
    python tools/c2dag/c2dag.py src/di_input.c di_UpdateButtons --alloc  # the register allocator's priorities

--alloc (what rea's c2prio printed, without Frida): per allocator block its weight `P blk N = count x loop
weight` and each counted reference (`ref use|def blk N line L candidate cost C`), then the candidates by priority.
priority = sum(cost * P) over the references - sum(P) over the blocks the candidate lives through unreferenced;
callee-saved registers go to the highest priorities, a tie to the candidate created later (the larger `tie`).
Every constant prints as const#0@ID: tell them apart by the @ID.

Per block the nodes are listed in their order BEFORE scheduling:
    idx op class line  prio  height in/out flags -> successor/latency:kind ...  | uses | defs
An operand is register, m:symbol (memory), k7 (immediate) or kb / ka (a call's memory use / kill), then its two
flag bytes (second byte: 10 last use). Edges come from these: a def after a use or def of the same thing.
The scheduler emits the ready node with the highest priority; a tie goes to the earlier node. prio is
height * 8192 (+ 65536 for a load: flag 04). height is the longest latency path to the block's end, so an
instruction only overtakes an earlier one when something later in the SAME block depends on it (an edge).
`ln` is the source line relative to the function's first line. op is C2's opcode number (OPS below names the
common ones; add what you identify).

How it works: build/c2dag/bin holds a copy of the compiler whose C2.DLL is tools/c2dag/c2proxy.c (compiled with
VC6 itself); the proxy loads the real back end as c2real.dll and hooks its priority pass. Nothing of the
toolchain install is modified. Paths: C2DAG_VC (the VC6 root), C2DAG_DX (the DirectX include directory).
"""
import argparse, os, re, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
HERE = os.path.dirname(os.path.abspath(__file__))
VC = os.environ.get("C2DAG_VC", r"C:\Tools\MSVC600-SP5PP")
DX = os.environ.get("C2DAG_DX", r"C:\Tools\DXSDK61\include")
WORK = os.path.join(ROOT, "build", "c2dag")
BIN = os.path.join(WORK, "bin")
# opcode numbers: partly from rea's c2prio (1 mov, d push, e call, f jcc, 10 jmp, 12 lea, 2f cmp, 2a0 return)
OPS = {0x1: "mov", 0xd: "push", 0xe: "call", 0xf: "jcc", 0x10: "jmp", 0x12: "lea", 0x2d: "add", 0x2f: "cmp",
       0x33: "test", 0x2a0: "ret", 0x2b0: "(end)", 0xffff: "(edge)"}


def env():
    e = dict(os.environ)
    e["INCLUDE"] = VC + r"\VC98\Include"
    e["LIB"] = VC + r"\VC98\Lib"
    e["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + e["PATH"]
    return e


def setup():
    src = os.path.join(HERE, "c2proxy.c")
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
                        "/Fo" + os.path.join(WORK, "c2proxy.obj"), "/link", "/BASE:0x20000000", "user32.lib"],
                       env=env(), cwd=WORK, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("building the proxy failed:\n" + r.stdout)
    shutil.copy2(os.path.join(WORK, "c2.dll"), dll)


def run(src, extra=(), blocks=False):
    setup()
    tag = os.path.splitext(os.path.basename(src))[0]
    dag = os.path.join(WORK, tag + ".dag")
    asm = os.path.join(WORK, tag + ".asm")
    if os.path.exists(dag):
        os.remove(dag)
    e = env()
    e["C2DAG_OUT"] = dag
    cmd = [os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy"] + (["/d2FAF"] if blocks else []) + list(extra) + \
          ["/I" + os.path.join(ROOT, "include"), "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fa" + asm,
           "/Fo" + os.path.join(WORK, tag + ".obj"), os.path.abspath(src)]
    r = subprocess.run(cmd, env=e, cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("compile failed:\n" + "\n".join(l for l in r.stdout.splitlines() if "error" in l))
    return dag, asm


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--ops", help="only blocks containing all of these opcode numbers (hex, comma separated)")
    ap.add_argument("--blocks", action="store_true", help="print VC6's block listing of the function instead")
    ap.add_argument("--flags", default="", help="extra cl flags")
    ap.add_argument("--alloc", action="store_true",
                    help="the register allocator instead: per block its weight P and the counted references, then "
                         "every candidate's priority (highest first gets a callee-saved register; ties: created later)")
    a = ap.parse_args()
    dag, asm = run(a.src, a.flags.split(), a.blocks)
    if a.blocks:
        on = False
        for l in open(asm, encoding="latin-1"):
            if a.func and re.match(r"[_@]%s(@\d+)?\s+PROC" % re.escape(a.func), l):
                on = True
            elif a.func and re.match(r"[_@]%s(@\d+)?\s+ENDP" % re.escape(a.func), l):
                on = False
            elif (on or not a.func) and (l.startswith("; Block(") or re.match(r"\t[a-z]|\$\w+(\$\d+)?:", l)):
                print(l.rstrip().split("\t;")[0])
        return
    if a.alloc:
        on = None
        cands = []
        regs = {}
        for l in open(dag, encoding="latin-1"):
            if l.startswith("allocrefs "):
                on = "refs" if re.sub(r"@\d+$", "", l.split()[1].lstrip("_@")) == a.func else None
            elif l.startswith("alloc "):
                on = "cands" if re.sub(r"@\d+$", "", l.split()[1].lstrip("_@")) == a.func else None
            elif on == "refs" and l.startswith((" ref ", " ablock ", " P ")):
                print(l.rstrip())
            elif on == "cands" and l.startswith(" cand "):
                m = re.match(r" cand (\S+) prio (-?\d+) tie (\w+)", l)
                cands.append((int(m.group(2)), int(m.group(3), 16), m.group(1)))
            elif l.startswith("allocreg "):
                on = "regs" if re.sub(r"@\d+$", "", l.split()[1].lstrip("_@")) == a.func else None
            elif on == "regs" and l.startswith(" cand "):
                m = re.match(r" cand (\S+) .* reg (\S+)", l)
                if m:
                    regs[m.group(1)] = m.group(2)
            elif on in ("cands", "regs"):
                on = None
        # the register each candidate got (none: it stays in memory / is rematerialised)
        for prio, tie, name in sorted(cands, reverse=True):
            print("  %-32s prio %5d  tie %3x  %s" % (name, prio, tie, regs.get(name, "-")))
        return
    want = set(int(x, 16) for x in a.ops.split(",")) if a.ops else None
    cur, keep = [], False
    out = []

    def flush():
        if cur and keep:
            ops = set(int(m.group(1), 16) for m in (re.search(r"op=(\w+)", l) for l in cur[1:]) if m)
            if not want or want <= ops:
                out.extend(cur)

    for l in open(dag, encoding="latin-1"):
        if l.startswith("block "):
            flush()
            cur = [l.rstrip()]
            keep = not a.func or re.search(r" func [_@]?%s(@\d+)? " % re.escape(a.func), l) is not None
        else:
            m = re.search(r"op=(\w+)", l)
            name = OPS.get(int(m.group(1), 16), "") if m else ""
            l = re.sub(r"ln=(-?\d+)", lambda k: "ln=%-4d" % (int(k.group(1)) & 0xffffff if int(k.group(1)) >= 0 else -1), l)
            cur.append(l.rstrip() + ("   ; " + name if name else ""))
    flush()
    print("\n".join(out))


if __name__ == "__main__":
    main()
