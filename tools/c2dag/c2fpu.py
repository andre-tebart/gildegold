"""VC6's x87 decisions for one function (C2 13.00.9044): which float values get an FPU stack slot, which live in
memory, and the stack at every x87 instruction with the pops.

    python tools/c2dag/c2fpu.py src/d3_io_t.c d3iot_CalcBounds            # candidates + the stack per line
    python tools/c2dag/c2fpu.py src/am.c ai_FireCannon --variants FILE --label L
    python tools/c2dag/c2fpu.py src/am.c ai_FireCannon --lines 11133-11150  # the code view of these lines only
    python tools/c2dag/c2fpu.py SRC FUNC --cands / --code                   # one of the two parts
    python tools/c2dag/c2fpu.py SRC FUNC --il                               # the proxy's raw dump (every stage)

Candidates: every float value the FP allocator (c2.dll FUN_1076697d, class 1) considered: a scalar local or a
field of a local aggregate (NAME+OFFSET, one per definition web), an unnamed CSE / expression temporary
(temp#N), a global or constant (const / k#N, kind 7: loaded at entry, never worth it). Per piece: its first
definition and death lines, prio / savings after the priority pass (FUN_10766d7b), and the outcome:
  stack                    FUN_10767ca5 placed it: an FPU stack slot from its definition to its death
  memory: savings S <= 0   FUN_10767401 dropped it before trying (S = sum of costs, see the rules)
  memory: no fit           FUN_10767ca5 refused it: its live range does not nest with a placed one (born later
                           but dying later), listed by lines; the allocator then splits it (FUN_10744286) and the
                           pieces come back as new rows (often one piece fits)
Rules (/O2, read from c2.dll): savings / priority per reference, times w = 1 << loop depth of the block:
  a 4-byte float: definition +2, a use 0, a dying `fld` +1; an 8-byte double: definition +4, use +1, dying fld +2;
  minus w for every CFG edge the value is live across out of a block; globals / constants: a load at entry -1 or
  -2. Savings must stay > 0: a float local live around a loop's back edge or into several blocks stays in memory,
  a double with uses in the loop gets the stack. Queue: priority descending, equal: the larger tie (+0x44)
  first. A value dies at its last use moved forward to the end of its statement's expression (no expression
  temporary left on the stack); there VC6 pops it (`fstp st(0)`) or, when the statement's result goes to a
  stack candidate, stores the result into the dying slot (`fstp st(k)`); the /O2 peephole FUN_10765f00 then turns
  `op st, st(k) ... fstp st(k)` into the popping `opp st(k), st` (the `fld st(0); fmulp` square).

Code view: the function's x87 instructions from VC6's listing of the same compile, per source line, with the
stack after each (st0 first; a memory value by its listing name, an expression result as ~LINE, a call's result
as ret@LINE) and the pops: `pop X` (the value is consumed), `dead pop X` (fstp st(0) of a value no one reads),
`into st(k)` (the top stored over a dead slot). The simulation follows the listing linearly; at a label reached
only by a jump it takes the stack of the jump.

How it works: tools/c2dag/c2fpu.c is a c2.dll proxy (built into build/c2dag/fpu/bin next to a compiler copy, the
toolchain is not modified) that hooks the FP allocator's queue (FUN_10767401 calls), the fit result (10766a88 /
10766ad2) and dumps the IL before each pass of the code generator FUN_1076e010; C2FPU_FUNC selects the function.
"""
import argparse, os, re, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
import c2trace  # noqa: E402

VC, DX = c2trace.VC, c2trace.DX
WORK = os.path.join(ROOT, "build", "c2dag", "fpu")
BIN = os.path.join(WORK, "bin")


def setup():
    src = os.path.join(HERE, "c2fpu.c")
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
                        "/Fo" + os.path.join(WORK, "c2fpu.obj"), "/link", "/BASE:0x20000000", "user32.lib"],
                       env=c2trace.env(), cwd=WORK, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("building the fpu proxy failed:\n" + r.stdout)
    shutil.copy2(os.path.join(WORK, "c2.dll"), dll)


CAND = re.compile(r"(Q|OK|NO) (\S+) prio (-?\d+) sav (-?\d+) tie (\w+) kind (\w+) size (-?\d+) defs(.*?) deaths(.*)$")


def pts(s):
    """'12:63 L1234 7:265' -> [12, None, 7] (the IL line of each point; a label is None)"""
    out = []
    for t in s.split():
        m = re.match(r"(-?\d+):", t)
        out.append(int(m.group(1)) if m else None)
    return out


class Piece:
    def __init__(self, m):
        self.key = m.group(2)
        self.name = re.sub(r"@[0-9a-f]+$", "", self.key)
        self.name = self.name[1:] if self.name.startswith("_") else self.name
        self.prio, self.sav, self.tie = int(m.group(3)), int(m.group(4)), int(m.group(5), 16)
        self.kind, self.size = int(m.group(6), 16), int(m.group(7))
        self.defs, self.deaths = pts(m.group(8)), pts(m.group(9))
        self.outcome = None

    def span(self):
        d = [x for x in self.defs if x is not None]
        e = [x for x in self.deaths if x is not None]
        return (min(d) if d else None), (max(e) if e else None)


def read_cands(path):
    """the pieces in queue order of their first appearance, with their outcome"""
    pieces, by, order = [], {}, []
    for l in open(path, encoding="latin-1"):
        m = CAND.match(l.rstrip("\n"))
        if not m:
            continue
        tag, key = m.group(1), m.group(2)
        p = by.get(key)
        if tag == "Q":
            if p is None or p.outcome is not None:   # a record address reused by a later piece
                p = Piece(m)
                by[key] = p
                pieces.append(p)
            else:
                p.prio, p.sav = int(m.group(3)), int(m.group(4))
        else:
            if p is None:
                p = Piece(m)
                by[key] = p
                pieces.append(p)
            p.outcome = "stack" if tag == "OK" else "no fit"
            order.append(p)
    for p in pieces:
        if p.outcome is None:
            p.outcome = "savings"
    return pieces, order


def conflicts(p, order):
    # a death before the definition (a loop's back edge, the death point at the loop head) makes the line
    # interval meaningless: no conflicts listed then
    """the placed pieces (before p) whose line interval p does not nest with: born before p and dying inside it,
    or born inside it and dying after it (by lines: an approximation of FUN_10767ca5's def / death point test)"""
    a, b = p.span()
    if a is None or b is None or b < a or min(x for x in p.deaths if x is not None) < a:
        return []
    out = []
    for q in order:
        if q is p:
            break
        if q.outcome != "stack":
            continue
        c, d = q.span()
        if c is None or d is None or d < c:
            continue
        if (c < a < d < b) or (a < c < b < d):
            out.append(q)
    return out


def cand_report(pieces, order, src):
    ln = src.ln
    print("FP candidates (first piece first; prio / savings after the priority pass):")
    w = max([len(p.name) for p in pieces] + [8])
    for p in pieces:
        a, b = p.span()
        ds = sorted({x for x in p.deaths if x is not None})
        rng = "%s-%s" % (ln(a) if a is not None else "?", ",".join(str(ln(x)) for x in ds) if ds else "?")
        if p.outcome == "stack":
            what = "stack"
        elif p.outcome == "savings":
            what = "memory: savings %d <= 0" % p.sav
        else:
            cs = conflicts(p, order)
            what = "memory: no fit" + (" (does not nest with %s)" % ", ".join(
                "%s %d-%d" % (q.name, ln(q.span()[0]), ln(q.span()[1])) for q in cs[:3]) if cs else
                " (stack depth, or nesting at a block edge)")
        kind = {3: "temp", 4: "local", 5: "param", 7: "global", 8: "global"}.get(p.kind, "kind %x" % p.kind)
        print("  %-*s %-14s %-6s size %d  prio %3d sav %3d  -> %s" % (w, p.name, rng, kind, p.size, p.prio, p.sav, what))
        if a is not None:
            print("  %-*s   %d: %s" % (w, "", ln(a), src.txt(a, 90)))


# ---- the code view: a stack simulation over VC6's listing

POP1 = {"fstp", "faddp", "fsubp", "fsubrp", "fmulp", "fdivp", "fdivrp", "fcomp", "fucomp", "fistp", "fbstp"}
ARITH = {"fadd", "fsub", "fsubr", "fmul", "fdiv", "fdivr", "faddp", "fsubp", "fsubrp", "fmulp", "fdivp", "fdivrp",
         "fiadd", "fisub", "fisubr", "fimul", "fidiv", "fidivr"}
UNARY = {"fchs", "fabs", "fsqrt", "fsin", "fcos", "frndint", "fscale", "f2xm1", "fptan", "fpatan", "fyl2x", "fprem"}
PUSH = {"fld", "fild", "fld1", "fldz", "fldpi", "fldl2e", "fldl2t", "fldlg2", "fldln2", "fbld"}


def opname(o):
    o = re.sub(r"\b(DWORD|QWORD|TBYTE|WORD) PTR\s*", "", o.strip())
    o = o.replace("FLAT:", "")
    return o[1:] if o.startswith("_") else o


def st_index(o):
    m = re.match(r"ST\((\d)\)$", o.strip(), re.I)
    return int(m.group(1)) if m else (0 if o.strip().upper() == "ST" else None)


def code_view(asm, func, src, lines=None, il_final=None):
    text = open(asm, encoding="latin-1").read()
    m = re.search(r"^([_@]?%s(@\d+)?)\s+PROC\b.*?$(.*?)^\1\s+ENDP" % re.escape(func), text, re.M | re.S)
    if not m:
        raise SystemExit("%s: not in the listing" % func)
    body = m.group(3).split("\n")
    def simulate(own):
        stack, saved, cur, dead_after_jmp = [], {}, None, False
        buf = []

        def show():
            return " ".join(stack) if stack else "-"

        def need(k):
            while len(stack) <= k:
                stack.append("ret@%s" % cur)

        for raw in body:
            l = raw.rstrip()
            mm = re.match(r";\s*(\d+)\s*:\s*(.*)$", l)
            if mm:
                cur = int(mm.group(1))
                buf.append(("src", cur, mm.group(2).strip()))
                continue
            lab = re.match(r"(\$\w+):", l)
            if lab:
                if dead_after_jmp and lab.group(1) in saved:
                    stack[:] = list(saved[lab.group(1)])
                dead_after_jmp = False
                buf.append(("lab", lab.group(1), show()))
                continue
            im = re.match(r"\s+(\w+)\s*(.*?)\s*(;.*)?$", l)
            if not im or l.lstrip().startswith(";"):
                continue
            mn, ops = im.group(1).lower(), im.group(2)
            args = [a for a in re.split(r",\s*(?![^\[]*\])", ops) if a] if ops else []
            if mn.startswith("j"):
                t = args[0].replace("SHORT", "").strip() if args else ""
                saved.setdefault(t, list(stack))
                dead_after_jmp = mn == "jmp"
                continue
            if mn == "ret":
                dead_after_jmp = True
                continue
            if mn == "call":
                note = ""
                if "__ftol" in ops:
                    need(0)
                    note = "pop %s (ftol)" % stack.pop(0)
                buf.append(("ins", cur, "call " + opname(ops), note, show()))
                continue
            if not mn.startswith("f") or mn in ("fnstsw", "fstsw", "fwait", "fldcw", "fnstcw", "fstcw"):
                continue
            note = ""
            kk = sum(1 for i in buf if i[0] == "ins")
            val = "~%s" % (own[kk] if own and kk < len(own) and own[kk] is not None else cur)
            if mn in PUSH:
                if mn == "fld" and args and st_index(args[0]) is not None:
                    k = st_index(args[0])
                    need(k)
                    stack.insert(0, stack[k])
                    note = "copy st(%d)" % k
                else:
                    stack.insert(0, opname(args[0]) if args else mn[2:] or mn)
            elif mn == "fxch":
                k = st_index(args[0]) if args else 1
                need(k)
                stack[0], stack[k] = stack[k], stack[0]
            elif mn in ("fst", "fstp", "fist", "fistp"):
                need(0)
                k = st_index(args[0]) if args else None
                if k is not None:
                    need(k)
                    if mn == "fstp" and k == 0:
                        note = "dead pop %s" % stack[0]
                        stack.pop(0)
                    else:
                        note = "into st(%d) (%s dead)" % (k, stack[k])
                        stack[k] = stack[0]
                        if mn == "fstp":
                            stack.pop(0)
                else:
                    if mn in ("fstp", "fistp"):
                        note = "pop %s" % stack.pop(0)
                    elif mn == "fst":
                        stack[0] = opname(args[0])     # the value is now also that memory's
            elif mn in ARITH:
                popping = mn in POP1
                ks = [st_index(a) for a in args]
                if len(args) == 2 and ks[0] is not None and ks[1] is not None:
                    d, s = ks
                elif len(args) == 1 and ks[0] is not None:
                    d, s = (0, ks[0])
                elif not args:
                    d, s = 1, 0
                else:
                    d, s = 0, None
                need(max(d, s or 0))
                if popping:
                    note = "pop %s" % stack[s if s is not None else 0]
                    stack[d] = val
                    stack.pop(0)
                else:
                    stack[d] = val
            elif mn in UNARY:
                need(0)
                stack[0] = val
            elif mn in ("fcom", "fucom"):
                need(0)
            elif mn in ("fcomp", "fucomp"):
                need(0)
                note = "pop %s" % stack.pop(0)
            elif mn in ("fcompp", "fucompp"):
                need(1)
                note = "pop %s, %s" % (stack.pop(0), stack.pop(0))
            elif mn in ("ficom", "ficomp"):
                need(0)
                if mn == "ficomp":
                    note = "pop %s" % stack.pop(0)
            buf.append(("ins", cur, ("%s %s" % (mn, ops)).strip(), note, show()))
        return buf

    buf = simulate(None)
    want = (lambda n: lines[0] <= n <= lines[1]) if lines else (lambda n: True)
    # each x87 instruction's own source line: the final IL (the proxy's `final` dump, in emitted order, a line
    # per instruction) aligned with the listing by mnemonic; the listing's `; N:` comments put scheduled code
    # under the last statement of a run, so they are only the fallback
    ins = [i for i in buf if i[0] == "ins"]
    own = [None] * len(ins)
    if il_final:
        import difflib
        a1 = [i[2].split()[0] for i in ins]
        b1 = [m for m, _ in il_final]
        sm = difflib.SequenceMatcher(None, a1, b1, autojunk=False)
        for blk in sm.get_matching_blocks():
            for k in range(blk.size):
                own[blk.a + k] = src.ln(il_final[blk.b + k][1])
        # an instruction whose line differs from two agreeing neighbours takes theirs (a few tuples carry the
        # line of the statement C1 first built them for)
        for k in range(1, len(own) - 1):
            if own[k - 1] is not None and own[k - 1] == own[k + 1] and own[k] != own[k - 1]:
                own[k] = own[k - 1]
        buf = simulate(own)
        ins = [i for i in buf if i[0] == "ins"]
    last = None
    for k, item in enumerate(ins):
        _, ln, text_, note, st = item
        n = own[k] if own[k] is not None else ln
        if n is not None and not want(n):
            continue
        if n != last:
            rel = n - src.ln(0) if n is not None else None
            print("%s: %s" % (n, src.txt(rel, 100) if rel is not None else ""))
            last = n
        print("    %-34s %-30s st: %s" % (text_[:34], note[:30], st[:120]))


def final_il(path, func):
    """[(mnemonic, IL line)] of the x87 instructions (and calls) in emitted order: the `final` dump's order, each
    tuple's line taken from the dump before FUN_10713cbe (which renumbers the
    lines of moved code like the listing does), matched by tuple address"""
    stages, cur = {}, None
    if not os.path.exists(path):
        return []
    for l in open(path, encoding="latin-1"):
        if l.startswith("@@ "):
            w = l.split()
            cur = None
            if w[1].lstrip("_@").split("@")[0] == func:
                cur = " ".join(w[2:])
                stages[cur] = []
            continue
        if cur is None:
            continue
        m = re.match(r"\s+[0-9a-f]+ c [0-9a-f]+ (-?\d+) \|.* ; (\w+) @([0-9a-f]+)$", l.rstrip())
        if m:
            stages[cur].append((m.group(2), int(m.group(1)), m.group(3)))
    fin = stages.get("final", [])
    pre = {a: (mn, ln) for mn, ln, a in stages.get("before 10713cbe", [])}
    out, last = [], None
    for mn, ln, a in fin:
        # a tuple the peepholes made is new (its address may be a freed one's: the mnemonic must agree);
        # it takes the line of the instruction before it
        p = pre.get(a)
        ln = p[1] if p and p[0] == mn else (last if last is not None else ln)
        last = ln
        if mn.startswith("f") or mn == "call":
            out.append((mn, ln))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--variants", help="a dcp try variants file: one of its variants instead of the tree's text")
    ap.add_argument("--label", help="the variant (default: the first)")
    ap.add_argument("--cands", action="store_true", help="only the candidates")
    ap.add_argument("--code", action="store_true", help="only the code view")
    ap.add_argument("--lines", help="A-B: the code view of these file lines only")
    ap.add_argument("--il", action="store_true", help="print the proxy's raw output (the IL at every stage)")
    a = ap.parse_args()
    setup()
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    tag = os.path.splitext(os.path.basename(a.src))[0]
    if a.variants:
        text, lab = c2trace.variant_text(text, a.func, a.variants, a.label, src=a.src)
        tag += "_" + re.sub(r"\W", "_", lab)
    c = os.path.join(WORK, tag + ".c")
    out = os.path.join(WORK, tag + ".fpu")
    asm = os.path.join(WORK, tag + ".asm")
    with open(c, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    if os.path.exists(out):
        os.remove(out)
    e = c2trace.env()
    e["C2FPU_OUT"] = out
    e["C2FPU_FUNC"] = a.func
    r = subprocess.run([os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/FAs",
                        "/I" + os.path.join(ROOT, "include"), "/I" + os.path.join(ROOT, "src"),
                        "/I" + os.path.dirname(os.path.abspath(a.src)), "/I" + DX, "/Fa" + asm,
                        "/Fo" + os.path.join(WORK, tag + ".obj"), c], env=e, cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("compile failed:\n" + "\n".join(l for l in r.stdout.splitlines() if "error" in l))
    if a.il:
        sys.stdout.write(open(out, encoding="latin-1").read() if os.path.exists(out) else "")
        return
    src = c2trace.Src(c, a.func)
    if not a.code:
        if not os.path.exists(out):
            print("%s: no output (not a function of this unit?)" % a.func)
        else:
            pieces, order = read_cands(out)
            if pieces:
                cand_report(pieces, order, src)
            else:
                print("%s: no FP candidates (every float value lives in memory between statements)" % a.func)
    if not a.cands:
        lines = tuple(int(x) for x in a.lines.split("-")) if a.lines else None
        if not a.code:
            print()
        print("x87 code (VC6's listing of this compile; st: the stack after the instruction, st0 first):")
        code_view(asm, a.func, src, lines, final_il(out, a.func))


if __name__ == "__main__":
    try:
        main()
    except OSError as e:
        if e.errno not in (22, 32):
            raise
