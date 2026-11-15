"""Which tails VC6 merged in one function, and into which copy (C2 13.00.9044, fg.c).

    python tools/c2dag/c2tail.py src/am.c ai_DecideUseAlchemyItems
    python tools/c2dag/c2tail.py src/am.c ai_DecideUseAlchemyItems --variants v.txt --label L
    python tools/c2dag/c2tail.py SRC FUNC --tries     # also every pair the mergers compared
    python tools/c2dag/c2tail.py SRC FUNC --asm       # also the function's listing
    python tools/c2dag/c2tail.py SRC FUNC --il merge  # the IL the mergers start from (pre / merge / post / all)
    C2DAG_TRYDUMP=1 python tools/c2dag/c2tail.py ..   # per `pair` try the compared instructions, the equal
                                                      # count and the size; raw lines in build/c2dag/UNIT.dag

Which block falls into the epilogue is decided before allocation (FUN_1073cc0d, c2pass --skip 0x10703443): the
blocks are laid out in reverse postorder of a depth-first walk from the entry that follows each block's JUMP
edge before its fall-through; the exit block goes last. The return block the walk finishes first (the end of
the chain of jump edges: the code after an `if (c) goto L;`, after an if body, after a loop, the then-part of
an `||`) is the one directly in front of the exit label. The C text order of blocks does not matter, only the
flow graph and which edge of each test is the jump.

The merging runs after register allocation and before scheduling, in the flow pass FUN_1070a5fe (up to 17
rounds, called with merging on from FUN_1070ec81). Three mergers, each ending in FUN_1071355b(deleted copy,
kept copy); the c2dag proxy hooks that call and writes `tmerge` records into build/c2dag/UNIT.dag:
  xjump  FUN_10712437  each `jmp L` against the code that falls into L: up to 30 instructions back on each
                       side (a conditional jump ends the region), matched as a dependency graph (any
                       instruction that can be last on both sides is sunk); the copy before the jmp is
                       deleted, the jmp goes to a new label in front of the kept copy. No size limit; repeated
                       rounds extend a merged tail back across an equal jcc.
  pair   FUN_1074633c  two `jmp L` to one label: compared linearly back from both jmps (same opcode, flags,
                       operands; a jcc equal when its target is; labels skipped). The copy of the jmp later in
                       L's reference list (newest reference first: usually the EARLIER jmp in the source; a
                       retargeted jmp moves to the front) is deleted, except when it is entered by fall-through
                       and the other one is a whole block behind a jmp / ret. Both entered by fall-through:
                       merged only when the equal run measures more than 22 (FUN_10710628's size: a 7-ins
                       mov / call tail 23, a 20-ins x87 / call tail 20).
  move   FUN_107711fb  the same pair for a label nothing falls into (behind a jmp / ret); no size limit; here the
                       FIRST reference's copy (usually the later jmp in the source) is deleted and the kept copy
                       is moved in front of the label.
Lines are source lines of SRC (the IL line of each instruction); opcodes as in c2dag.py (1 mov, e call, ..).
"""
import argparse, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import c2dag, c2trace  # noqa: E402

WORK = os.path.join(c2dag.ROOT, "build", "c2dag", "tail")


def opname(op):
    n = c2dag.OPS.get(int(op, 16))
    return n if n else "op" + op


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--variants", help="a dcp try variants file: one of its variants instead of the tree's text")
    ap.add_argument("--label", help="the variant (default: the first)")
    ap.add_argument("--tries", action="store_true", help="also every pair compared (xjump / pair / move)")
    ap.add_argument("--asm", action="store_true", help="also the function's listing")
    ap.add_argument("--il", choices=("pre", "merge", "post", "all"),
                    help="the IL (labels, jumps, instructions with source lines) before the first flow pass after "
                         "allocation (pre), before the merging pass (merge), after it (post)")
    ap.add_argument("--flags", default="", help="extra cl flags")
    a = ap.parse_args()
    os.environ["C2DAG_IL"] = a.func if a.il else ""
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    tag = os.path.splitext(os.path.basename(a.src))[0]
    if a.variants:
        text, lab = c2trace.variant_text(text, a.func, a.variants, a.label, src=a.src)
        tag += "_" + re.sub(r"\W", "_", lab)
    os.makedirs(WORK, exist_ok=True)
    c = os.path.join(WORK, tag + ".c")
    with open(c, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    lines = text.split("\n")
    _, s, _ = c2trace.function_span(lines, a.func)
    dag, asm = c2dag.run(c, a.flags.split())

    def src(ln):
        ln = int(ln)
        k = s + ln
        return k + 1, (lines[k].strip() if 0 <= k < len(lines) else "?")

    def run_text(run):
        items = [(o, ln) for o, ln in re.findall(r"(\w+)@(-?\d+)", run) if o != "2b0"]  # 2b0: a block mark
        ops = " ".join(opname(o) for o, _ in items) + (" .." if run.rstrip().endswith("..") else "")
        lns = []
        for _, ln in items:
            if not lns or lns[-1] != ln:
                lns.append(ln)
        return ops, [src(ln) for ln in lns]

    me = re.compile(r"[_@]?%s(@\d+)?$" % re.escape(a.func))
    recs = open(dag, encoding="latin-1").read().split("\n")
    n = 0
    i = 0
    while i < len(recs):
        l = recs[i]
        if l.startswith("tmerge ") and me.match(l.split()[1]):
            who = l.split()[3]
            dele, kept = recs[i + 1].split(":", 1)[1], recs[i + 2].split(":", 1)[1]
            n += 1
            for title, run in (("deleted", dele), ("kept   ", kept)):
                ops, srcs = run_text(run)
                if title == "deleted":
                    print("merge %d by %s: %d instructions" % (n, who, len(ops.replace(" ..", "").split())))
                print("  %s %s" % (title, ops))
                for ln, t in srcs:
                    print("      %5d  %s" % (ln, t[:100]))
            i += 3
            continue
        if a.tries and l.startswith("ttry ") and me.match(l.split()[1]):
            p = l.split()
            (la, ta), (lb, tb) = src(p[3].split("@")[1]), src(p[4].split("@")[1])
            print("try %s: jmp at %d (%s) against jmp at %d (%s), both to %s" % (p[2], la, ta[:40], lb, tb[:40],
                                                                              p[5] if len(p) > 5 else "?"))
        if a.tries and l.startswith("xjump ") and me.match(l.split()[1]):
            ln, t = src(l.split("@")[-1])
            print("try xjump: jmp at %d (%s) against the code falling into its label" % (ln, t[:50]))
        i += 1
    if not n:
        print("no tail merged in %s" % a.func)
    if a.il:
        on, last = False, None
        for l in recs:
            if l.startswith("til "):
                st = l.split()[2]
                on = me.match(l.split()[1]) is not None and a.il in (st, "all")
                if on:
                    print("--- IL %s" % st)
                    last = None
                continue
            if not on:
                continue
            if not l.startswith(" "):
                on = False
                continue
            if l.startswith(" L"):
                print(l.rstrip())
                last = None
                continue
            p = l.split()
            op, cls, ln = p[0], p[1], p[2]
            tgt = " ".join(p[3:])
            name = opname(op)
            if ln != last:
                k, t = src(ln)
                txt = "   ; %d  %s" % (k, t[:70])
                last = ln
            else:
                txt = ""
            print("    %-6s %-12s%s" % (name, tgt, txt))
    if a.asm:
        on = False
        for l in open(asm, encoding="latin-1"):
            if re.match(r"[_@]%s(@\d+)?\s+PROC" % re.escape(a.func), l):
                on = True
            elif re.match(r"[_@]%s(@\d+)?\s+ENDP" % re.escape(a.func), l):
                on = False
            elif on and re.match(r"\t[a-z]|\$\w+(\$\d+)?:", l):
                print(l.rstrip().split("\t;")[0])


if __name__ == "__main__":
    main()
