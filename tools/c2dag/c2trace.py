"""The register allocator's decisions for one function, step by step (C2 13.00.9044).

    python tools/c2dag/c2trace.py src/main.c main_ChooseHistory            # the assignment sequence
    python tools/c2dag/c2trace.py src/main.c main_ChooseHistory --raw       # every trace line
    python tools/c2dag/c2trace.py src/main.c FUNC --variants FILE [--label L]  # dcp try variants (whole text or blocks)
    python tools/c2dag/c2trace.py src/am.c ai_DecideUpgradeGuild --prio '^_(o1|money)$'   # priority per block
    python tools/c2dag/c2trace.py src/am.c ai_FillNearRing --split '_g'      # how g was split, the regions

--prio: each block's term of the priority (refcost * P * w where referenced, -P * w where the candidate lives
through unreferenced), with the references and their source lines: which block lifts or sinks a candidate.
--split: the candidate's queue / growth (GROW) / cuts (CUT, with the source line) / avail restrictions
(RESTRICT: the AF7 drop, with its reason) / assignments, then the bulk split's block decisions (REGION: the
block's predecessors in list order, the region it joined). Raw kinds added for this: GROW, CUT, RESTRICT,
REGION? / REGION= (C2TRACE_OPS=1 adds each block's IL ops), PRI (with --prio).

The variants file is read exactly as dcp try reads it (parse_variants / variant_text, also used by c2frame.py and
vdiff.py): `<<<` old `===` new `>>>` blocks, markers indented or not, replace text inside the function only (each
old text once in it); a body without blocks is the function's whole new text.

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
  * splitting (Chow-Hennessy): one candidate (FUN_107453f8 -> FUN_10745670) grows pieces from the first live block
    into successors while one register stays free over the piece; 3+ candidates at once in a function with 100+
    candidates (FUN_10743bda): blocks in layout order join the region of their FIRST predecessor (then
    successor) that leaves a register free for both, and every region boundary cuts all of them. A piece ends at
    its last reference: it lives across the region's calls only when it is referenced on both sides.
  * AF7 drop (0x107097cd): at a call the candidate is live across, or where one byte register is left for a live
    byte value, avail &= { ebp esi edi }.

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
        return j + 1 < len(lines) and lines[j + 1].startswith("{")
    s = next(i for i, l in enumerate(lines) if re.match(r"\w[\w \*]*\b%s\(" % re.escape(func), l) and not code(l).endswith(";")
             and opens_body(i))
    e = next(i for i in range(s, len(lines)) if lines[i].rstrip() == "}")
    cs = s
    while cs > 0 and not lines[cs - 1].startswith(("}", "#", ";")) and lines[cs - 1].strip():
        cs -= 1
    return cs, s, e


def parse_variants(vt):
    """a dcp try variants file read as dcp try reads it (decompanion src/variants.rs): {label: edit}, in file order.
    A variant starts with `### label`; its body (blank lines around it dropped) is either the function's whole new
    text or, when its first line is `<<<` (white space around any marker is ignored), `<<<` old `===` new `>>>`
    blocks: each old text must occur exactly once in the function's text. edit = ("whole", text) or ("pairs",
    [(old, new)])."""
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
        if body[0].strip() != "<<<":
            out[label] = ("whole", "\n".join(body))
            return
        pairs, k = [], 0
        while k < len(body):
            if not body[k].strip():
                k += 1
                continue
            if body[k].strip() != "<<<":
                raise SystemExit("variant %r: %r outside a <<< === >>> block" % (label, body[k]))
            mid = next((i for i in range(k, len(body)) if body[i].strip() == "==="), None)
            if mid is None:
                raise SystemExit("variant %r: <<< without ===" % label)
            end = next((i for i in range(mid, len(body)) if body[i].strip() == ">>>"), None)
            if end is None:
                raise SystemExit("variant %r: === without >>>" % label)
            old = "\n".join(body[k + 1:mid])
            if not old.strip():
                raise SystemExit("variant %r: an empty old text" % label)
            pairs.append((old, "\n".join(body[mid + 1:end])))
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
        for old, new in edit:
            n = fn.count(old)
            if n != 1:
                raise SystemExit("variant %r: the old text %r.. occurs %d times in the function (once needed)" % (
                    label, next((l.strip() for l in old.split("\n") if l.strip()), "")[:60], n))
            fn = fn.replace(old, new, 1)
    else:
        fn = edit
    return "\n".join(lines[:cs] + [fn] + lines[e + 1:]), label


def src_lines(c, func):
    """{trace line: source text}: C2's line numbers count from the function's definition line"""
    lines = open(c, encoding="latin-1").read().split("\n")
    _, d, e = function_span(lines, func)
    return {i - d: lines[i].strip() for i in range(d, e + 1)}


def prio_report(dag, func, pat, T, srcl):
    """The priority pass (FUN_10707a0b), per basic block b in layout order, for each candidate c live into b or
    referenced in b: referenced: prio += (sum of the costs of c's references in b) * P(b) * w(b); live through
    unreferenced: prio -= P(b) * w(b); dead (a definition follows on every path): nothing. P(b) = the number of
    distinct candidates (and other allocatable values) referenced in b, w(b) = 1 << loop depth under /O2. A
    reference costs 2 (FUN_107083c2), a constant less where the other operand is a register. The QUEUE line's
    priority is taken after the pass (FUN_10708777 adjusts it slightly)."""
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
                "%s%s:%s" % ("d" if m.group(1) == "def" else "u", m.group(3), m.group(5)))
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
    rx = re.compile(pat)
    names = []
    for r in rows:
        if rx.search(r[0].split("@")[0]) and r[0] not in names:
            names.append(r[0])
    if not names:
        raise SystemExit("no candidate matches %r (have: %s)" % (
            pat, " ".join(sorted({r[0].split("@")[0] for r in rows}))[:400]))
    for n in names:
        print("%s   (prio += refcost*P*w where referenced, -= P*w where it lives through unreferenced)" % n)
        print("  %4s %-9s %4s %3s %6s %6s  %s" % ("blk", "lines", "P", "w", "delta", "prio", "references (u/d line:cost)"))
        for name, b, d, v in rows:
            if name != n:
                continue
            lo, hi = lines.get(b, (0, 0))
            p, w = P.get(b, (0, 0))
            r = refs.get((n, b), [])
            txt = ("| " + srcl.get(int(r[-1][1:].split(":")[0]), "")[:60]) if r else ""
            print("  %4d %-9s %4d %3d %+6d %6d  %s %s" % (b, "%d-%d" % (lo, hi), p, w, d, v, " ".join(r), txt))
        q = [l for l in T if l.startswith("T QUEUE " + n.split("@")[0] + "@")]
        if q:
            print("  queued:", re.sub(r" avail.*", "", q[0][8:]))
        print()


def split_report(pat, T, srcl):
    """the candidates' way through the allocator, with the source line of each cut / restriction, then the bulk
    split's region decisions (FUN_10743bda) of the blocks where the regions were decided"""
    rx = re.compile(r"(^|\s)(%s)@" % pat)
    for l in T:
        if l.startswith("T REGION"):
            continue
        if rx.search(l) and re.match(r"T (QUEUE|SPLIT|  CUT|GROW|  RESTRICT|ASSIGN|  ->|UNPROFITABLE|NOREG)", l):
            m = re.search(r"line (\d+)", l)
            extra = ("   | " + srcl.get(int(m.group(1)), "")[:60]) if m and l.startswith(("T   CUT", "T   RESTRICT")) else ""
            print(l[:200] + extra)
    reg = [l for l in T if l.startswith("T REGION?")]
    if reg:
        print("bulk split (FUN_10743bda) block decisions; used = registers of assigned candidates in the block; a "
              "block joins the region of its FIRST predecessor, then successor, that leaves a register free for both:")
        for k, l in enumerate(T):
            if not l.startswith("T REGION?"):
                continue
            m = re.match(r"T REGION\? blk@(\w+)\[(\d+)-(\d+) ", l)
            if not m or not srcl.get(int(m.group(3))):
                continue
            nxt = T[k + 1] if k + 1 < len(T) and T[k + 1].startswith("T REGION= blk@" + m.group(1)) else ""
            r = re.search(r"r(\d+)\]", nxt)
            body = re.sub(r" r(5[89]|6\d|7[0-3])\b", "", l[10:])
            body = re.sub(r" ops .*", "", body)
            print("  %s -> %s   | %s" % (body[:200], "r" + r.group(1) if r else "no region",
                                         srcl[int(m.group(3))][:50]))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src"); ap.add_argument("func")
    ap.add_argument("--variants", help="a dcp try variants file: trace one of its variants instead of the tree's text")
    ap.add_argument("--label", help="the variant (default: the first)")
    ap.add_argument("--raw", action="store_true", help="every T line of the trace")
    ap.add_argument("--allpass", action="store_true", help="also the re-prioritisation passes after splits")
    ap.add_argument("--prio", metavar="REGEX", help="the priority breakdown per block of the candidates whose name "
                    "matches (e.g. '^_(o1|money)$')")
    ap.add_argument("--split", metavar="REGEX", help="how the candidates whose name matches were split: queue, "
                    "growth, cuts, restrictions, the pieces' registers and the bulk split's regions")
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
    if a.allpass:
        e["C2TRACE_ALLPASS"] = "1"
    if a.prio:
        e["C2TRACE_PRIO"] = "1"
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
    if a.prio:
        prio_report(dag, a.func, a.prio, T, src_lines(c, a.func))
        return
    if a.split:
        split_report(a.split, T, src_lines(c, a.func))
        return
    # the summary: the lines that queue, choose, assign and split (the kinds the proxy writes; see c2trace.c)
    keep = re.compile(r"T (QUEUE|ASSIGN|SPLIT|UNPROFITABLE|NOREG)\b")
    shown = [l for l in T if keep.match(l)]
    print("\n".join(shown if shown else T))
    print("(%d trace lines in %s; --raw prints all)" % (len(T), dag))


if __name__ == "__main__":
    main()
