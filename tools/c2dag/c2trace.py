"""The register allocator's decisions for one function (C2 13.00.9044): every candidate piece, what became of it.

    python tools/c2dag/c2trace.py src/d3_fl.c d3fl_DrawWater               # the summary per source name
    python tools/c2dag/c2trace.py src/main.c FUNC --variants FILE [--label L]  # the same for one dcp try variant
    python tools/c2dag/c2trace.py src/am.c ai_DecideUpgradeGuild --prio 'o1|money'   # priority per block
    python tools/c2dag/c2trace.py src/am.c ai_FillNearRing --split g        # how g was split, the regions
    python tools/c2dag/c2trace.py src/main.c FUNC --seq / --raw             # the event sequence / every trace line

Summary (the default): one header line (basic blocks, allocation passes, whether the bulk split ran and why
(not)), then per source name (variables and globals first, then temporaries temp#N / k#N / const#N with the
source line that first uses them) its final registers and one row per candidate piece: NAME#N, the priority pass
that made it, the source lines of its references, prio / sav / avail at its first queue insert, and the outcome:
a register, `memory: savings S < 1`, `cut by pressure where X#n took R` (the pass restarts and the name comes
back as a new piece), `no register left where X#n took R: split alone / in regions` (with the pieces and the cut
lines), `not popped` (the pass restarted first). Line numbers are the traced text's file lines (with --variants:
the copy under build/c2dag/trace).

Piece names: NAME#N counts the candidates of one source name in the order the trace first shows them (each
priority pass re-creates the unassigned candidates; a split makes new ones). They are the same in every mode
(summary, --split, --prio, --seq) of one text, so rows can be matched across outputs; the proxy's own
name@addr (--raw) is a heap address and differs between runs.

Name REGEX (--prio, --split): matched against the plain name (map), the compiler's (_map; temp#677, k#48f) and the
piece (map#2); first as a whole match (`t|npoints|map` selects t, npoints and map, `^_(tx|ret)$` works too), and
only when nothing matches whole, as a search. Nothing matching at all: an error listing the function's names.

--prio: each block's term of the first pass's priority (refcost * P * w where referenced, -P * w where the
candidate lives through unreferenced), with the references and their source lines: which block lifts or sinks a
candidate; `queued:` is the piece's own queue insert and outcome.
--split: the pieces' rows, their queue / growth (GROW) / cuts (CUT, with the source line) / avail restrictions
(RESTRICT: the AF7 drop, with its reason) / assignments, then each bulk split call that had one of them (its
candidates; when it split in regions, the block decisions: the block's predecessors in list order, the region it
joined). Raw kinds added for this: GROW, CUT, RESTRICT, BULK (the bulk split's entry: set size, the basic block
count, regions or one-by-one), BLOCKS (the basic block count per pass), REGION? / REGION= (C2TRACE_OPS=1 adds
each block's IL ops), PRI (with --prio).

The variants file is read exactly as dcp try reads it (parse_variants / variant_text, also used by c2frame.py and
vdiff.py): `<<<` old `===` new `>>>` blocks, markers indented or not, replace text inside the function only (each
old text once in it); a body without blocks is the function's whole new text.

`c2dag.py --alloc` prints the candidates after the FIRST priority pass and the register each one ended with. That
is not enough to predict an outcome: assigning a register can split other candidates, and the pieces re-enter the
queue with new priorities. This tool follows the pieces through every pass.

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
  * splitting (Chow-Hennessy): the neighbours an assignment leaves without a register go to FUN_10743bda. Fewer
    than 3 of them, or a function of fewer than 100 basic blocks (the count at 0x107ad5b8): each alone
    (FUN_107453f8 -> FUN_10745670), growing pieces from the first live block into successors while one register
    stays free over the piece. 3+ in a function of 100+ basic blocks (the bulk split): blocks in layout order join
    the region of their FIRST predecessor (then successor) that leaves a register free for both, and every region
    boundary cuts all of them. A piece ends at its last reference: it lives across the region's calls only when it
    is referenced on both sides.
  * AF7 drop (0x107097cd): at a call the candidate is live across, or where one byte register is left for a live
    byte value, avail &= { ebp esi edi }.

How it works: tools/c2dag/c2trace.c is the c2dag proxy with more hooks (the queue insert, the choice, the
assignment, the splits, the bulk split's entry); it is built into build/c2dag/trace/bin next to a copy of the
compiler. C2TRACE_FUNC selects the function (its exact name), C2TRACE_ALLPASS (always set) prints every pass's
references, the `T ...` lines go to the C2DAG_OUT file.
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


def code(l):
    """a line without its comments (a prototype's trailing `/* 0x507690 */` must not hide its `;`)"""
    return re.sub(r"/\*.*?\*/|//.*$", "", l).rstrip()


def function_span(lines, func):
    """(the first line of the address comment above the definition, the definition's first line, its closing `}`)"""
    def opens_body(i):
        # the definition's parameter list may wrap over several lines before the "{"
        j = i
        while j < len(lines) and not code(lines[j]).endswith(")"):
            if code(lines[j]).endswith((";", "{", "}")):
                return False
            j += 1
        k = j + 1
        # a K&R definition: `f(a, b)`, then the parameters' declarations, then the "{"
        while k < len(lines) and re.match(r"[A-Za-z_][\w \*,\[\]]*;$", code(lines[k])):
            k += 1
        return k < len(lines) and lines[k].startswith("{")
    s = next(i for i, l in enumerate(lines) if re.match(r"\w[\w \*]*\b%s\(" % re.escape(func), l) and not code(l).endswith(";")
             and opens_body(i))
    e = next(i for i in range(s, len(lines)) if lines[i].rstrip() == "}")
    cs = s
    while cs > 0 and not lines[cs - 1].startswith(("}", "#", ";")) and lines[cs - 1].strip():
        cs -= 1
    return cs, s, e


OPEN = re.compile(r"<<<(\d+|\*)?$")   # a block's opening marker: `<<<`, `<<<N` (the N-th occurrence), `<<<*` (all)


def parse_variants(vt):
    """a dcp try variants file read as dcp try reads it (decompanion src/variants.rs): {label: edit}, in file order.
    A variant starts with `### label`; its body (blank lines around it dropped) is either the function's whole new
    text or, when its first line is `<<<` (white space around any marker is ignored), `<<<` old `===` new `>>>`
    blocks: each old text must occur exactly once in the function's text, unless the opening marker picks
    one: `<<<N` the N-th occurrence (counted in the text the earlier blocks left), `<<<*` every one. edit =
    ("whole", text) or ("pairs", [(old, new, which)]), which None (exactly once), N or "*"."""
    out, cur = {}, None
    def finish():
        if cur is None:
            return
        label, body = cur
        while body and not body[-1].strip():
            body.pop()
        while body and not body[0].strip():
            body.pop(0)
        if not body:
            raise SystemExit("variant %r is empty" % label)
        if label in out:
            raise SystemExit("a second variant %r" % label)
        if not OPEN.match(body[0].strip()):
            out[label] = ("whole", "\n".join(body))
            return
        pairs, k = [], 0
        while k < len(body):
            if not body[k].strip():
                k += 1
                continue
            om = OPEN.match(body[k].strip())
            if not om:
                raise SystemExit("variant %r: %r outside a <<< === >>> block" % (label, body[k]))
            which = om.group(1) and (om.group(1) if om.group(1) == "*" else int(om.group(1)))
            if which == 0:
                raise SystemExit("variant %r: <<<0 (occurrences count from 1)" % label)
            mid = next((i for i in range(k, len(body)) if body[i].strip() == "==="), None)
            if mid is None:
                raise SystemExit("variant %r: <<< without ===" % label)
            end = next((i for i in range(mid, len(body)) if body[i].strip() == ">>>"), None)
            if end is None:
                raise SystemExit("variant %r: === without >>>" % label)
            old = "\n".join(body[k + 1:mid])
            if not old.strip():
                raise SystemExit("variant %r: an empty old text" % label)
            pairs.append((old, "\n".join(body[mid + 1:end]), which or None))
            k = end + 1
        out[label] = ("pairs", pairs)
    for l in vt.replace("\r\n", "\n").split("\n"):
        if l.startswith("###"):
            finish()
            cur = (l[3:].strip(), [])
        elif cur is not None:
            cur[1].append(l)
        elif l.strip():
            raise SystemExit("text before the first ### label: %r" % l)
    finish()
    return out


def dcp_span(src, func):
    """(comment start, end) of the function in src as dcp itself delimits it (`dcp fn`: at=FILE:A-B, 1-based), or
    None when dcp does not know the function in this file (a scratch copy)"""
    try:
        r = subprocess.run(["dcp", "fn", func], cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=300)
    except (OSError, subprocess.SubprocessError):
        return None
    m = re.search(r"\bat=(\S+):(\d+)-(\d+)", r.stdout)
    if not m or not os.path.exists(os.path.join(ROOT, m.group(1))) or not os.path.exists(src) \
            or not os.path.samefile(os.path.join(ROOT, m.group(1)), src):
        return None
    return int(m.group(2)) - 1, int(m.group(3)) - 1


def variant_text(text, func, vfile, label, src=None):
    """the unit's text with one variant of a dcp try file applied, and the variant's label (default: the first).
    As in dcp try, blocks replace text inside the function only and a body without blocks is the function's whole
    new text (address comment to closing brace). src: the file the text was read from; when it is the tree's file
    the function is delimited by `dcp fn`, else by function_span."""
    lines = text.split("\n")
    span = dcp_span(src, func) if src else None
    if span:
        cs, e = span
    else:
        cs, _, e = function_span(lines, func)
    vs = parse_variants(open(vfile, encoding="latin-1").read())
    if not vs:
        raise SystemExit("no variants in %s" % vfile)
    label = label or next(iter(vs))
    if label not in vs:
        raise SystemExit("no variant %r (have: %s)" % (label, ", ".join(vs)))
    kind, edit = vs[label]
    fn = "\n".join(lines[cs:e + 1])
    if kind == "pairs":
        for old, new, which in edit:
            n = fn.count(old)
            first = next((l.strip() for l in old.split("\n") if l.strip()), "")[:60]
            if which == "*":
                if not n:
                    raise SystemExit("variant %r: the old text %r.. does not occur in the function" % (label, first))
                fn = fn.replace(old, new)
            elif which:
                if n < which:
                    raise SystemExit("variant %r: <<<%d but the old text %r.. occurs %d times in the function" % (
                        label, which, first, n))
                at, start = -1, 0
                for _ in range(which):
                    at = fn.index(old, start)
                    start = at + len(old)
                fn = fn[:at] + new + fn[at + len(old):]
            else:
                if n != 1:
                    raise SystemExit("variant %r: the old text %r.. occurs %d times in the function (once needed)" % (
                        label, first, n))
                fn = fn.replace(old, new, 1)
    else:
        fn = edit
    return "\n".join(lines[:cs] + [fn] + lines[e + 1:]), label


def src_lines(c, func):
    """{trace line: source text}: C2's line numbers count from the function's definition line"""
    lines = open(c, encoding="latin-1").read().split("\n")
    _, d, e = function_span(lines, func)
    return {i - d: lines[i].strip() for i in range(d, e + 1)}


class Src:
    """the traced text: C2 counts a line from the function's definition line (0); printed as the text's file line"""
    def __init__(self, c, func):
        lines = open(c, encoding="latin-1").read().split("\n")
        _, self.d, e = function_span(lines, func)
        self.text = {i - self.d: lines[i].strip() for i in range(self.d, e + 1)}

    def ln(self, rel):
        return self.d + rel + 1

    def txt(self, rel, w=60):
        return self.text.get(rel, "")[:w]


# a candidate as the proxy prints it: name@addr (a variable or global `_name`, an unnamed temporary `temp#N`, a
# constant `const#N`, another value `k#N`); addr is the low 16 bits of the allocator's record, reused across passes
KEY = re.compile(r"(?<![\w#@])((?:temp|const|k)#[0-9a-f]+|[A-Za-z_][\w$?]*)@([0-9a-f]+)\b")


class Piece:
    """one allocator candidate (a whole variable or one piece of it) in one allocation pass, named NAME#N: N counts
    the candidates of that source name in the order the trace first shows them (the same in every mode)"""
    def __init__(self, raw, n, pass_):
        self.raw, self.pass_ = raw, pass_
        self.name = raw[1:] if raw.startswith("_") else raw
        self.label = "%s#%d" % (self.name, n)
        self.lines, self.queues, self.notes = [], [], []
        self.reg = self.final = None
        self.assigned = self.terminal = self.popped = self.bulked = False
        self.pieces, self.cuts = 0, []

    def note(self, s):
        if s not in self.notes:
            self.notes.append(s)

    def span(self, src):
        ls = sorted({l for l in self.lines if l >= 0})
        if not ls:
            return "lines -"
        if len(ls) <= 6:
            return "lines %s (%d refs)" % (",".join(str(src.ln(l)) for l in ls), len(self.lines))
        return "lines %d-%d (%d refs)" % (src.ln(ls[0]), src.ln(ls[-1]), len(self.lines))

    def outcome(self):
        if self.assigned:
            s = self.reg
        elif self.final == "-":
            s = "memory"
        elif self.notes:
            s = ""
        elif self.popped:
            s = "?"
        else:
            s = "not popped: the pass was restarted (another candidate's pressure split)"
        return "; ".join([x for x in [s] + self.notes if x])


def matches(rx, p, whole=True):
    """REGEX against the forms of a name: the plain source name (map), the compiler's (_map; temp#677, k#48f,
    const#0 as they are) and the piece (map#2): `map`, `^map$`, `^_map$`, `map#2` all select map. whole: the
    regex must match a whole form (`t|npoints|map` selects t, npoints and map, not water); else a search"""
    f = rx.fullmatch if whole else rx.search
    return bool(f(p.name) or f(p.raw) or f(p.label))


class Trace:
    """the trace of one function read into pieces. The trace runs with C2TRACE_ALLPASS: every allocation pass
    prints its references (allocrefs), so each piece has its source lines and a pass number."""
    def __init__(self, dag, func, src):
        self.src, self.pieces, self.events, self.bulk = src, [], [], []
        self.blocks, self.passes, self.final, self.pri = None, 0, [], []
        self.by_pass = {}
        cur, stale, count = {}, set(), {}
        last_assigned = [None]

        def new(key, raw):
            count[raw] = count.get(raw, 0) + 1
            p = Piece(raw, count[raw], self.passes)
            self.pieces.append(p)
            cur[key] = p
            stale.discard(key)
            self.by_pass[(self.passes, key)] = p
            return p

        def get(m, ctx=None):
            key = m.group(0)
            p = cur.get(key)
            if p is None or (ctx and key in stale) or (ctx == "queue" and p.terminal):
                p = new(key, m.group(1))
            return p

        def label(l):
            return KEY.sub(lambda m: cur[m.group(0)].label if m.group(0) in cur and m.group(1) != "blk" else m.group(0), l)

        def subject(l):
            if l.startswith("T   RESTRICT"):
                l = l.split(":", 1)[1]
            for m in KEY.finditer(l):
                if m.group(1) != "blk":
                    return m
            return None

        def isfunc(nm):
            return nm.lstrip("_@").split("@")[0] == func
        on, final = False, False
        for l in open(dag, encoding="latin-1"):
            l = l.rstrip("\n")
            if l.startswith("allocrefs "):
                if isfunc(l.split()[-1]):
                    on, self.passes = True, self.passes + 1
                    stale = {k for k, p in cur.items() if not p.assigned}
                elif on:
                    break
                continue
            if l.startswith("allocreg "):
                final = on and isfunc(l.split()[-1])
                continue
            m = re.match(r"T BLOCKS (\d+)", l)
            if m:
                self.blocks = int(m.group(1))
                continue
            if not on:
                continue
            if final:
                m = re.match(r" cand (\S+) .* reg (\S+)", l)
                k = m and KEY.match(m.group(1))
                if k and k.group(0) in cur:
                    cur[k.group(0)].final = m.group(2)
                    self.final.append(cur[k.group(0)])
                continue
            m = re.match(r" ref (use|def) blk \d+ line (-?\d+) (\S+) cost", l)
            if m:
                k = KEY.match(m.group(3))
                if k:
                    get(k, "ref").lines.append(int(m.group(2)))
                continue
            if not l.startswith("T "):
                continue
            if l.startswith("T PRI "):
                self.pri.append((self.passes, l))
                continue
            kind = l[2:].split()[0]
            s = subject(l)
            p = None
            if kind == "QUEUE" and s:
                p = get(s, "queue")
                q = re.search(r"prio (-?\d+) sav (-?\d+) refs (-?\d+) .*?avail\{([^}]*)\}", l)
                if q:
                    p.queues.append((int(q.group(1)), int(q.group(2)), int(q.group(3)), q.group(4).split()))
            elif s:
                p = get(s)
            if kind in ("AF7", "ASSIGN") and p:
                p.popped = True
            elif kind == "->" and p:
                r = re.search(r" reg (\w+)", l)
                p.reg, p.assigned, p.terminal, p.popped = r.group(1) if r else "?", True, True, True
                last_assigned[0] = p
            elif kind == "scores":
                m = re.search(r"pressure-split\{([^}]*)\}", l)
                a = last_assigned[0]
                for k in KEY.finditer(m.group(1) if m else ""):
                    if k.group(0) in cur:
                        cur[k.group(0)].note("cut by pressure where %s took %s" % (a.label, a.reg) if a else "cut by pressure")
            elif kind == "UNPROFITABLE" and p:
                q = re.search(r" sav (-?\d+)", l)
                p.terminal, p.popped = True, True
                p.note("memory: savings %s < 1" % (q.group(1) if q else "?"))
            elif kind == "NOREG-AT-TURN" and p:
                p.popped = True
                p.note("no register at its turn")
            elif kind == "SPLIT" and p:
                p.terminal, p.popped = True, True
                if not p.bulked:
                    p.note("split at its turn: no register left (avail empty)")
            elif kind == "GROW" and p:
                q = re.search(r" piece (\d+)", l)
                p.pieces = max(p.pieces, int(q.group(1)) if q else 0)
            elif kind == "CUT" and p:
                q = re.search(r"line (-?\d+)", l)
                if q:
                    p.cuts.append(int(q.group(1)))
            elif kind == "BULK":
                m = re.match(r"T BULK n (\d+) blocks (\d+) -> (\S+)", l)
                if m:
                    members = [cur[k.group(0)] for k in KEY.finditer(l) if k.group(0) in cur]
                    self.bulk.append({"n": int(m.group(1)), "blocks": int(m.group(2)), "mode": m.group(3),
                                      "members": members, "pass": self.passes, "regions": []})
                    a = last_assigned[0]
                    for x in members:
                        x.bulked = x.terminal = True
                        x.note("no register left where %s took %s: split %s" % (
                            a.label if a else "?", a.reg if a else "?",
                            "in regions (bulk split)" if m.group(3) == "regions" else "alone (FUN_107453f8)"))
            elif kind in ("REGION?", "REGION=") and self.bulk:
                self.bulk[-1]["regions"].append(l)
            self.events.append((kind, p, label(l)))
        for p in self.pieces:
            if p.pieces:
                cuts = ",".join(str(src.ln(c)) for c in sorted(set(p.cuts)))
                p.note("%d pieces%s" % (p.pieces, ", cut at line " + cuts if cuts else ""))

    def queued(self):
        return [p for p in self.pieces if p.queues]

    def bulk_why(self):
        """whether the bulk split (FUN_10743bda) split in regions in this function, and why (not)"""
        b = self.blocks
        if not self.bulk:
            return ("bulk split: never called (no assignment left a neighbour without a register)%s"
                    % ("" if b is None else "; %d basic blocks (regions need 100+)" % b))
        reg = [x for x in self.bulk if x["mode"] == "regions"]
        one = [x for x in self.bulk if x["mode"] != "regions"]
        why = []
        if one:
            few = sum(1 for x in one if x["n"] < 3)
            small = sum(1 for x in one if x["blocks"] < 100)
            why.append("%d one by one (%s)" % (len(one), ", ".join(filter(None, [
                "%d with fewer than 3 candidates" % few if few else "",
                "%d basic blocks < 100" % one[0]["blocks"] if small else ""]))))
        if reg:
            why.append("%d in regions (3+ candidates, %d basic blocks >= 100)" % (len(reg), reg[0]["blocks"]))
        return "bulk split (FUN_10743bda: 3+ candidates left without a register by one assignment and 100+ basic " \
               "blocks): called %d times: %s" % (len(self.bulk), "; ".join(why))

    def row(self, p):
        q = p.queues[0] if p.queues else None
        qs = ("prio %d sav %d avail{%s}" % (q[0], q[1], " ".join(q[3]))) if q else "not queued"
        if len(p.queues) > 1:
            qs += " (requeued: prio %d)" % p.queues[-1][0]
        return "  %-14s pass %-2d %-26s %-36s -> %s" % (p.label, p.pass_, p.span(self.src), qs, p.outcome())

    def select(self, pat, pieces=None):
        rx = re.compile(pat)
        pool = pieces if pieces is not None else self.queued()
        sel = [p for p in pool if matches(rx, p)] or [p for p in pool if matches(rx, p, False)]
        order = {}
        for p in sel:
            order.setdefault(p.name, len(order))
        sel.sort(key=lambda p: order[p.name])
        if not sel:
            names = sorted({p.name for p in self.queued()})
            raise SystemExit("no candidate matches %r (tried whole and as a search against the plain name, the "
                             "compiler's name and the piece label); the function's candidates: %s" % (pat, " ".join(names)))
        return sel


def summary(tr, func):
    """per source name: its pieces with priority, savings, avail at the queue, the source lines of their
    references, and what became of each (register, memory, split, cut by pressure)"""
    print("%s: %s basic blocks, %d allocation passes; %s" % (func, tr.blocks, tr.passes, tr.bulk_why()))
    print("lines: of the traced text %s; pass: the priority pass that made the piece; prio/sav/avail: at its first "
          "queue insert; final: the pieces holding a register at the end (the others live in memory)" % tr.src_path)
    groups = {}
    for p in tr.queued():
        groups.setdefault(p.name, []).append(p)
    named = [n for n in groups if not re.match(r"(temp|const|k)#", n)]
    other = [n for n in groups if n not in named]
    for n in named + other:
        fin = ["%s %s" % (p.label, p.final) for p in tr.final if p.name == n and p.final != "-"]
        what = ""
        if n not in named:
            first = min((l for p in groups[n] for l in p.lines if l >= 0), default=None)
            what = "   (line %d: %s)" % (tr.src.ln(first), tr.src.txt(first, 50)) if first is not None else ""
        print("%s   final: %s%s" % (n, ", ".join(fin) if fin else "memory", what))
        for p in groups[n]:
            print(tr.row(p))


def prio_report(dag, func, pat, tr):
    """The priority pass (FUN_10707a0b) of the first allocation pass, per basic block b in layout order, for each
    candidate c live into b or referenced in b: referenced: prio += (sum of the costs of c's references in b) *
    P(b) * w(b); live through unreferenced: prio -= P(b) * w(b); dead (a definition follows on every path):
    nothing. P(b) = the number of distinct candidates (and other allocatable values) referenced in b, w(b) = 1 <<
    loop depth under /O2. A reference costs 2 (FUN_107083c2), a constant less where the other operand is a
    register. The QUEUE line's priority is taken after the pass (FUN_10708777 adjusts it slightly)."""
    src = tr.src
    rows, P, refs, lines = [], {}, {}, {}
    state = 0
    for l in open(dag, encoding="latin-1"):
        l = l.rstrip()
        if l.startswith("allocrefs "):
            if state == 1:
                break
            state = 1 if (state == 0 and l.split()[-1].lstrip("_") == func) else (2 if state else 0)
            continue
        if l.startswith("alloc ") and state == 1:
            break
        if state != 1:
            continue
        m = re.match(r" ref (use|def) blk (\d+) line (-?\d+) (\S+) cost (-?\d+)", l)
        if m:
            refs.setdefault((m.group(4), int(m.group(2))), []).append(
                "%s%s:%s" % ("d" if m.group(1) == "def" else "u", src.ln(int(m.group(3))), m.group(5)))
            continue
        m = re.match(r" ablock (\d+) lines (\d+)-(\d+)", l)
        if m:
            lines[int(m.group(1))] = (int(m.group(2)), int(m.group(3)))
            continue
        m = re.match(r" P blk (\d+) = (-?\d+) x (\d+)", l)
        if m:
            P[int(m.group(1))] = (int(m.group(2)), int(m.group(3)))
            continue
        m = re.match(r"T PRI (\S+) id \w+ blk (\d+) ([+-]\d+) = (-?\d+)", l)
        if m:
            rows.append((m.group(1), int(m.group(2)), int(m.group(3)), int(m.group(4))))
    first = {k: p for (n, k), p in tr.by_pass.items() if n == 1}
    keys = []
    for r in rows:
        if r[0] in first and r[0] not in keys:
            keys.append(r[0])
    sel = tr.select(pat, [first[k] for k in keys])
    for p in sel:
        k = next(k for k in keys if first[k] is p)
        print("%s (%s; pass 1)   (prio += refcost*P*w where referenced, -= P*w where it lives through "
              "unreferenced)" % (p.label, p.span(src)))
        print("  %4s %-9s %4s %3s %6s %6s  %s" % ("blk", "lines", "P", "w", "delta", "prio", "references (u/d line:cost)"))
        for name, b, d, v in rows:
            if name != k:
                continue
            lo, hi = lines.get(b, (0, 0))
            pw, w = P.get(b, (0, 0))
            r = refs.get((k, b), [])
            txt = ("| " + src.txt(int(r[-1][1:].split(":")[0]) - src.d - 1)) if r else ""
            bl = "%d-%d" % (src.ln(lo), src.ln(hi)) if lo <= hi else "-"
            print("  %4d %-9s %4d %3d %+6d %6d  %s %s" % (b, bl, pw, w, d, v, " ".join(r), txt))
        if p.queues:
            q = p.queues[0]
            print("  queued: %s prio %d sav %d refs %d avail{ %s } -> %s" % (p.label, q[0], q[1], q[2], " ".join(q[3]),
                                                                          p.outcome()))
        print()


def split_report(pat, tr):
    """the candidates' way through the allocator, with the source line of each cut / restriction, then the bulk
    split's calls and (when it split in regions) its region decisions per block (FUN_10743bda)"""
    src = tr.src
    sel = tr.select(pat)
    print(tr.bulk_why())
    for p in sel:
        print(tr.row(p))
    print()
    for kind, p, l in tr.events:
        if p not in sel or kind not in ("QUEUE", "SPLIT", "CUT", "GROW", "RESTRICT", "ASSIGN", "->", "UNPROFITABLE",
                                        "NOREG-AT-TURN"):
            continue
        m = re.search(r"line (-?\d+)", l)
        if m and kind in ("CUT", "RESTRICT"):
            l = l.replace(m.group(0), "line %d" % src.ln(int(m.group(1)))) + "   | " + src.txt(int(m.group(1)))
        elif kind == "GROW":
            l = re.sub(r"lines (-?\d+)-(-?\d+)", lambda q: "lines %d-%d" % (src.ln(int(q.group(1))), src.ln(int(q.group(2))))
                       if int(q.group(1)) <= int(q.group(2)) else "lines -", l)
        print(l[:220])
    for b in tr.bulk:
        if not any(p in sel for p in b["members"]):
            continue
        print("bulk split in pass %d: %d candidates, %d basic blocks -> %s: %s" % (
            b["pass"], b["n"], b["blocks"], b["mode"], " ".join(p.label for p in b["members"])[:400]))
        if b["mode"] != "regions":
            continue
        print("  block decisions; used = registers of assigned candidates in the block; a block joins the region of "
              "its FIRST predecessor, then successor, that leaves a register free for both:")
        R = b["regions"]
        for k, l in enumerate(R):
            if not l.startswith("T REGION?"):
                continue
            m = re.match(r"T REGION\? blk@(\w+)\[(\d+)-(\d+) ", l)
            if not m or not src.text.get(int(m.group(3))):
                continue
            nxt = R[k + 1] if k + 1 < len(R) and R[k + 1].startswith("T REGION= blk@" + m.group(1)) else ""
            r = re.search(r"r(\d+)\]", nxt)
            body = re.sub(r" r(5[89]|6\d|7[0-3])\b", "", l[10:])
            body = re.sub(r" ops .*", "", body)
            body = re.sub(r"\[(\d+)-(\d+) ", lambda q: "[%d-%d " % (src.ln(int(q.group(1))), src.ln(int(q.group(2)))), body)
            print("  %s -> %s   | %s" % (body[:200], "r" + r.group(1) if r else "no region", src.txt(int(m.group(3)), 50)))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--variants", help="a dcp try variants file: trace one of its variants instead of the tree's text")
    ap.add_argument("--label", help="the variant (default: the first)")
    ap.add_argument("--raw", action="store_true", help="every T line of the trace (the proxy's own name@addr names)")
    ap.add_argument("--seq", action="store_true", help="the events in order (queue, assign, split, unprofitable, "
                    "bulk split), names as pieces")
    ap.add_argument("--allpass", action="store_true", help="(always on; accepted for old command lines)")
    ap.add_argument("--prio", metavar="REGEX", help="the priority breakdown per block (first pass) of the candidates "
                    "whose name matches (map, ^map$, ^_map$, map#1 all work)")
    ap.add_argument("--split", metavar="REGEX", help="how the candidates whose name matches were split: queue, "
                    "growth, cuts, restrictions, the pieces' registers, the bulk split's calls and regions")
    a = ap.parse_args()
    setup()
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    tag = os.path.splitext(os.path.basename(a.src))[0]
    if a.variants:
        text, lab = variant_text(text, a.func, a.variants, a.label, src=a.src)
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
    e["C2TRACE_ALLPASS"] = "1"  # every pass's references: the pieces' source lines and the pass boundaries
    if a.prio:
        e["C2TRACE_PRIO"] = "1"
    r = subprocess.run([os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/I" + os.path.join(ROOT, "include"),
                        "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fa" + os.path.join(WORK, tag + ".asm"),
                        "/Fo" + os.path.join(WORK, tag + ".obj"), c], env=e, cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("compile failed:\n" + "\n".join(l for l in r.stdout.splitlines() if "error" in l))
    T = [l.rstrip() for l in open(dag, encoding="latin-1") if l.startswith("T ")]
    if not T:
        lst = open(os.path.join(WORK, tag + ".asm"), encoding="latin-1").read()
        if re.search(r"^[_@]%s(@\d+)?\s+PROC" % re.escape(a.func), lst, re.M):
            print("%s: no register candidates (the allocator had nothing to choose; registers follow the scratch "
                  "rotation and the scheduler alone)" % a.func)
            return
        raise SystemExit("no trace lines: %s is not a function of this unit's listing" % a.func)
    if a.raw:
        print("\n".join(T))
        return
    tr = Trace(dag, a.func, Src(c, a.func))
    tr.src_path = os.path.relpath(c, ROOT) if a.variants else a.src
    if a.prio:
        prio_report(dag, a.func, a.prio, tr)
    elif a.split:
        split_report(a.split, tr)
    elif a.seq:
        keep = ("QUEUE", "ASSIGN", "SPLIT", "UNPROFITABLE", "NOREG-AT-TURN", "BULK")
        print("\n".join(l for k, p, l in tr.events if k in keep))
    else:
        summary(tr, a.func)
    print("(%d trace lines in %s; --raw prints all, --seq the sequence)" % (len(T), os.path.relpath(dag, ROOT)))


if __name__ == "__main__":
    try:
        main()
    except OSError as e:    # the output piped into head: Windows reports EINVAL, POSIX EPIPE
        if e.errno not in (22, 32):
            raise
