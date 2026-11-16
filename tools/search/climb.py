"""Climb: an unattended search for the source form of one function (a beam search over source mutations).

    python tools/search/climb.py NAME                       # from the tree's text, until byte or --rounds
    python tools/search/climb.py NAME --rounds 40 --beam 8 --width 160
    python tools/search/climb.py NAME --base FILE           # start from another whole-function text
    python tools/search/climb.py NAME --ops swap,move,local  # only these operators (see OPS)
    python tools/search/climb.py NAME --resume              # go on from the last run's beam (build/climb/NAME.json)

Why: a worker tries a few hundred variants of a function by hand; the last hunks of a hard function need the
right COMBINATION of two or three ordinary changes (a named local here, one statement moved there), and no single
change shows a gain. This tool mutates the function's text, measures every mutant with `dcp try` (distance in
differing instructions, then bytes), keeps the best few and mutates those again, for as many rounds as it is
given.

The frame size counts: when a mutant's frame differs in size from the original's, dcp try aligns the stack
offsets before counting (its row says `frame 0x428 vs 0x424 (stack offsets aligned)`), so the distance looks far
smaller than it is. From a start whose frame has the original's size such mutants are dropped (each round prints
how many); from a start with another size they rank after every mutant that reaches the original's size, and a
best text that still has another size is marked `frame OURS vs ORIGINAL (offsets aligned: not comparable)`.

It is sound for one reason: a mutant that compiles to the original's bytes HAS the original's meaning. The
mutations on the way need not keep the meaning, and many do not (a statement moved past one it depends on); they
only have to lead somewhere. So nothing here is applied to the tree. The result is a text (build/climb/NAME.best.c)
and its diff against the tree's function; a byte result is then written into the source by hand, as the plain
code it suggests (a mutant can reach byte through an ugly route: an unused declaration, a redundant parenthesis -
drop what is not needed with a last `dcp try` round).

The operators are the levers that matched functions before (notes/vc6-quick.md): statement order, a named local
for a value read twice, a local written out at its uses, an assignment moved into the expression that uses it, a
statement after an if / else written in both arms and back, if / else arms swapped, a ternary as if / else and
back, operand order, parentheses around a member read, an initialiser as a statement. It does not know C: it
works on lines and tokens, and a mutant that does not compile costs one row.
"""
import argparse, difflib, hashlib, json, os, random, re, subprocess, sys, tempfile

ROOT = os.getcwd()
while not os.path.exists(os.path.join(ROOT, "decompanion.toml")) and os.path.dirname(ROOT) != ROOT:
    ROOT = os.path.dirname(ROOT)
OUT = os.path.join(ROOT, "build", "climb")
KEYWORD = re.compile(r"^\s*(if|else|for|while|do|switch|case|default|return|goto|break|continue|\}|\{|#)")
MEMBER = re.compile(r"(?<![\w\.>\]])([A-Za-z_]\w*(?:(?:->|\.)[A-Za-z_]\w*|\[[A-Za-z_0-9]\w*(?: [+\-] \d+)?\])+)(?!\s*\()")
IDENT = re.compile(r"[A-Za-z_]\w*")
# a local's type: int and float keep the meaning of nearly every member read; narrower or unsigned types change
# comparisons and truncate, so they are only tried with --types (and a hit then needs reading against the original)
TYPES = ["int", "float"]


def dcp(*args, timeout=3600):
    r = subprocess.run(["dcp"] + list(args), cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=timeout)
    return r.stdout + r.stderr


def function_of(name):
    """(the comment's lines, the definition's lines) of the tree's function"""
    out = dcp("fn", name).split("\n")
    lines = [l.rstrip("\r") for l in out[1:]]
    while lines and not lines[-1].strip():
        lines.pop()
    k = 0
    while k < len(lines) and "*/" not in lines[k]:
        k += 1
    return lines[:k + 1], lines[k + 1:]


def indent(l):
    return len(l) - len(l.lstrip(" "))


def simple(l):
    """a one-line statement: ends with `;`, no control keyword, parentheses balanced"""
    s = l.strip()
    return bool(s) and s.endswith(";") and not KEYWORD.match(l) and s.count("(") == s.count(")") and not s.startswith("/*") and ":" not in s.split("?")[0][-1:]


def is_decl(l):
    return bool(re.match(r"^\s+(?:(?:struct|union|unsigned|signed|const|static|register)\s+)*[A-Za-z_]\w*[\s\*]+[A-Za-z_]\w*(?:\[[^\]]*\])*(?:\s*=[^;]*)?(?:\s*,\s*\**[A-Za-z_]\w*(?:\[[^\]]*\])*(?:\s*=[^;]*)?)*;\s*(/\*.*\*/)?\s*$", l)) \
        and not re.match(r"^\s+(return|goto|else|break|continue)\b", l)


def body_start(lines):
    """index of the first line after the function's opening brace and its top declarations"""
    k = 0
    while k < len(lines) and lines[k].strip() != "{":
        k += 1
    k += 1
    first = k
    while k < len(lines) and (is_decl(lines[k]) or not lines[k].strip()):
        k += 1
    return first, k


UNSAFE = False                                            # --unsafe: moves and swaps without the check below
WORD = re.compile(r"\b[A-Za-z_]\w*")
EXIT = re.compile(r"\b(return|break|continue|goto|case|default)\b|^\s*[A-Za-z_]\w*:\s*$")
TARGET = re.compile(r"(\*?\s*[A-Za-z_][\w\.]*(?:->[\w\.]+|\[[^\]=]*\])*)\s*(?:[-+*/|&^%]|<<|>>)?=(?!=)")


def _targets(l):
    """what a statement writes, as access paths: `m.time`, `*o2`, `p->x`; an element write is its array (`q[`), a
    ++ / -- operand its name"""
    out = set()
    for m in TARGET.finditer(l):
        t = re.sub(r"\s+", "", m.group(1))
        out.add(re.sub(r"\[.*", "[", t))
    for m in re.finditer(r"(?:\+\+|--)\s*([A-Za-z_][\w\.]*(?:->[\w\.]+)*)|([A-Za-z_][\w\.]*(?:->[\w\.]+)*)\s*(?:\+\+|--)", l):
        out.add(m.group(1) or m.group(2))
    return out


def _touches(t, l):
    """does line l read or write what target t names (or the whole of which t is a part)"""
    base = WORD.search(t).group(0)
    if t == base or t.startswith("*"):
        return re.search(r"\b%s\b" % re.escape(base), l) is not None
    if t.endswith("["):
        return t in re.sub(r"\s+", "", l)
    flat = re.sub(r"\s+", "", l)
    if t in flat:
        return True
    # the whole object used or written as such (`m = s`, `&m`, `f(m)`): the name not followed by a member access
    return re.search(r"\b%s\b(?!\s*(\.|->|\[))" % re.escape(base), l) is not None


def _call(l):
    l = re.sub(r"\b(if|while|for|switch|return|sizeof)\s*\(", "(", l)
    return re.search(r"\b[A-Za-z_]\w*\s*\(", l) is not None


def _memory(l):
    """a store through a pointer, an element, a field or a global-looking name (what a call may read or write)"""
    return any(("->" in t or "[" in t or "." in t or t.startswith("*") or "_" in t) for t in _targets(l))


def independent(a, crossed):
    """can statement a move over the crossed lines without changing the meaning: no exit or label among them, no
    access path one writes and the other uses, no two calls, no call against a store through memory"""
    if UNSAFE:
        return True
    ta, ca, ma = _targets(a), _call(a), _memory(a)
    for c in crossed:
        s = c.strip()
        if not s or s.startswith(("/*", "//", "{", "}")):
            continue
        if EXIT.search(c):
            return False
        tc, cc = _targets(c), _call(c)
        if any(_touches(t, c) for t in ta) or any(_touches(t, a) for t in tc):
            return False
        if (ca and (cc or _memory(c))) or (cc and ma):
            return False
    return True


# --- the operators: each yields (label, new lines) for the sites it finds -------------------------------------

def op_swap(lines, rng):
    for i in range(len(lines) - 1):
        a, b = lines[i], lines[i + 1]
        if simple(a) and simple(b) and indent(a) == indent(b) and not is_decl(a) and not is_decl(b) and independent(a, [b]):
            yield "swap@%d" % i, lines[:i] + [b, a] + lines[i + 2:]


def op_move(lines, rng):
    for i, a in enumerate(lines):
        if not simple(a) or is_decl(a):
            continue
        for d in (-6, -5, -4, -3, -2, 2, 3, 4, 5, 6):
            j = i + d
            if j < 1 or j >= len(lines):
                continue
            lo, hi = min(i, j), max(i, j)
            if any(indent(lines[k]) < indent(a) and lines[k].strip() for k in range(lo, hi + 1)):
                continue                                  # do not leave the block
            crossed = [lines[k] for k in range(lo, hi + 1) if k != i]
            if not UNSAFE and sum(l.count("{") - l.count("}") for l in crossed) != 0:
                continue                                  # would land inside a block (a loop body runs it again)
            if not independent(a, crossed):
                continue                                  # the move would change the meaning
            rest = lines[:i] + lines[i + 1:]
            at = j if d < 0 else j
            if not UNSAFE and at > 0 and KEYWORD.match(rest[at - 1]) and not rest[at - 1].rstrip().endswith((";", "{", "}")):
                continue                                  # would become the body of a braceless if / else / loop
            if is_decl(rest[min(at, len(rest) - 1)]) and d < 0:
                continue
            yield "move@%d%+d" % (i, d), rest[:at] + [a] + rest[at:]


def op_commute(lines, rng):
    pat = re.compile(r"(?<![\w\.>\)\]])([\w\.\->\[\]]+) (\+|\*|&|\||\^|==|!=|<=|>=|<|>) ([\w\.\->\[\]]+)(?![\w\.\(\[])")
    flip = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}
    for i, l in enumerate(lines):
        if l.strip().startswith("for (") or is_decl(l):
            continue
        for m in pat.finditer(l):
            a, o, b = m.groups()
            if a == b or a.endswith("-") or "->" == a[-2:]:
                continue
            new = l[:m.start()] + "%s %s %s" % (b, flip.get(o, o), a) + l[m.end():]
            yield "commute@%d:%d" % (i, m.start()), lines[:i] + [new] + lines[i + 1:]


def op_compound(lines, rng):
    for i, l in enumerate(lines):
        m = re.match(r"^(\s*)([\w\.\->\[\]]+) = \2 ([+\-*|&]) (.*);$", l)
        if m:
            yield "compound@%d" % i, lines[:i] + ["%s%s %s= %s;" % (m.group(1), m.group(2), m.group(3), m.group(4))] + lines[i + 1:]
        m = re.match(r"^(\s*)([\w\.\->\[\]]+) ([+\-*|&])= (.*);$", l)
        if m:
            rhs = m.group(4) if re.match(r"^[\w\.\->\[\]]+$", m.group(4)) else "(%s)" % m.group(4)
            yield "expand@%d" % i, lines[:i] + ["%s%s = %s %s %s;" % (m.group(1), m.group(2), m.group(2), m.group(3), rhs)] + lines[i + 1:]


def fresh(lines):
    used = set(IDENT.findall("\n".join(lines)))
    for n in ("t", "v", "k", "n", "p", "q", "r", "s", "u", "w"):
        if n not in used:
            return n
    k = 0
    while "t%d" % k in used:
        k += 1
    return "t%d" % k


def op_local(lines, rng):
    """a named local for a member / element read that occurs twice or more: assigned before the statement of its
    first use, used in the next few lines"""
    first, start = body_start(lines)
    text = "\n".join(lines[start:])
    seen = {}
    for m in MEMBER.finditer(text):
        seen.setdefault(m.group(1), 0)
        seen[m.group(1)] += 1
    name = fresh(lines)
    wr = r"\s*(=[^=]|\+=|-=|\+\+|--|\*=|\|=|&=)"
    for expr, n in seen.items():
        if n < 2:
            continue
        idx = [i for i in range(start, len(lines)) if expr in lines[i]]
        for a0 in idx:
            if re.search(re.escape(expr) + wr, lines[a0]):
                continue                                  # this use writes it
            # the statement boundary before this use: back up over a multi-line statement's continuation lines
            i0 = a0
            while i0 > start and not (lines[i0 - 1].rstrip().endswith((";", "{", "}")) or (KEYWORD.match(lines[i0 - 1]) and lines[i0 - 1].rstrip().endswith((")", "else")))):
                i0 -= 1
            if KEYWORD.match(lines[i0]) and lines[i0].strip().startswith(("else", "}", "case", "default")):
                continue
            done = set()
            for span in (3, 10, 30, 400):
                last = max(k for k in idx if a0 <= k and k - a0 <= span)
                if last in done or (last == a0 and lines[a0].count(expr) < 2):
                    continue
                done.add(last)
                for ty in TYPES:
                    new = list(lines)
                    for k in idx:
                        if a0 <= k <= last:
                            new[k] = re.sub(r"(?<![\w\.>])" + re.escape(expr) + r"(?![\w\[]|" + wr + ")", name, new[k])
                    new.insert(i0, " " * indent(lines[i0]) + "%s = %s;" % (name, expr))
                    new.insert(first, "    %s%s%s;" % (ty, "" if ty.endswith("*") else " ", name))
                    yield "local:%s@%d:%s:%d" % (expr, a0, ty, last - a0), new

def op_inline(lines, rng):
    """a local assigned once by a one-line statement, written out at its uses"""
    first, start = body_start(lines)
    for i in range(start, len(lines)):
        m = re.match(r"^\s+([A-Za-z_]\w*) = (.*);$", lines[i])
        if not m or not simple(lines[i]):
            continue
        v, e = m.groups()
        pat = re.compile(r"(?<![\w\.>])" + re.escape(v) + r"(?![\w])")
        writes = [k for k in range(start, len(lines)) if re.search(r"(?<![\w\.>])" + re.escape(v) + r"\s*(=[^=]|\+=|-=|\+\+|--|\*=|\|=|&=)", lines[k]) or re.search(r"&" + re.escape(v) + r"\b", lines[k])]
        uses = [k for k in range(i + 1, len(lines)) if pat.search(lines[k])]
        if writes != [i] or not (1 <= len(uses) <= 4) or pat.search(e):
            continue
        rep = e if re.match(r"^[\w\.\->\[\]]+$", e) else "(%s)" % e
        new = [pat.sub(lambda _: rep, l) if k in uses else l for k, l in enumerate(lines)]
        del new[i]
        yield "inline:%s@%d" % (v, i), new


def op_assign_in_expr(lines, rng):
    """`v = e;` directly before the one statement that reads v: `(v = e)` in that statement"""
    for i in range(len(lines) - 1):
        m = re.match(r"^\s+([A-Za-z_][\w\.\->\[\]]*) = (.*);$", lines[i])
        if not m or not simple(lines[i]) or is_decl(lines[i]):
            continue
        v, e = m.groups()
        pat = re.compile(r"(?<![\w\.>])" + re.escape(v) + r"(?![\w\[\.]|->|\s*(?:=[^=]|\+=|-=|\+\+|--))")
        nxt = lines[i + 1]
        if len(pat.findall(nxt)) >= 1 and not nxt.strip().startswith(("for (", "while (", "do")):
            new = pat.sub(lambda _: "(%s = %s)" % (v, e), nxt, count=1)
            yield "inexpr:%s@%d" % (v, i), lines[:i] + [new] + lines[i + 2:]


def block_end(lines, i):
    """the index of the line that closes the block opened on line i (a line ending with `{`)"""
    depth = 0
    for k in range(i, len(lines)):
        depth += lines[k].count("{") - lines[k].count("}")
        if depth == 0 and k > i:
            return k
        if depth == 0 and k == i and not lines[k].rstrip().endswith("{"):
            return None
    return None


def op_arms(lines, rng):
    """a statement after an if { } else { } written at the end of both arms, and the reverse"""
    for i, l in enumerate(lines):
        if not re.match(r"^\s+if \(.*\) \{$", l):
            continue
        e = block_end(lines, i)
        if e is None or not re.match(r"^\s+\} else \{$", lines[e]) or indent(lines[e]) != indent(l):
            continue
        f = block_end(lines, e)
        if f is None or lines[f].strip() != "}":
            continue
        pad = " " * (indent(l) + 4)
        if f + 1 < len(lines) and simple(lines[f + 1]) and indent(lines[f + 1]) == indent(l) and not is_decl(lines[f + 1]):
            s = lines[f + 1].strip()
            yield "botharms@%d" % i, lines[:e] + [pad + s] + lines[e:f] + [pad + s] + [lines[f]] + lines[f + 2:]
        if e - 1 > i and f - 1 > e and lines[e - 1].strip() == lines[f - 1].strip() and simple(lines[e - 1]):
            s = lines[e - 1].strip()
            yield "joinarms@%d" % i, lines[:e - 1] + lines[e:f - 1] + [lines[f], " " * indent(l) + s] + lines[f + 1:]


def op_ifelse(lines, rng):
    """if (c) A; else B;  ->  if (!(c)) B; else A;   and   x = c ? a : b;  <->  if (c) x = a; else x = b;"""
    for i in range(len(lines) - 3):
        m = re.match(r"^(\s+)if \((.*)\)$", lines[i])
        if m and simple(lines[i + 1]) and lines[i + 2].strip() == "else" and simple(lines[i + 3]) and indent(lines[i + 2]) == len(m.group(1)):
            c = m.group(2)
            neg = c[2:-1] if c.startswith("!(") and c.endswith(")") else "!(%s)" % c
            yield "swaparms@%d" % i, lines[:i] + ["%sif (%s)" % (m.group(1), neg), lines[i + 3], lines[i + 2], lines[i + 1]] + lines[i + 4:]
            a = re.match(r"^\s+([\w\.\->\[\]]+) = (.*);$", lines[i + 1])
            b = re.match(r"^\s+([\w\.\->\[\]]+) = (.*);$", lines[i + 3])
            if a and b and a.group(1) == b.group(1):
                yield "ternary@%d" % i, lines[:i] + ["%s%s = %s ? %s : %s;" % (m.group(1), a.group(1), c, a.group(2), b.group(2))] + lines[i + 4:]
    for i, l in enumerate(lines):
        m = re.match(r"^(\s+)([\w\.\->\[\]]+) = ([^?;]+) \? ([^:;]+) : ([^;]+);$", l)
        if m:
            p, x, c, a, b = m.groups()
            c = c[1:-1] if c.startswith("(") and c.endswith(")") and c.count("(") == 1 else c
            yield "ifelse@%d" % i, lines[:i] + ["%sif (%s)" % (p, c), "%s    %s = %s;" % (p, x, a), "%selse" % p, "%s    %s = %s;" % (p, x, b)] + lines[i + 1:]


def op_paren(lines, rng):
    first, start = body_start(lines)
    for i in range(start, len(lines)):
        l = lines[i]
        if "=" not in l:
            continue
        for m in MEMBER.finditer(l):
            if l[:m.start()].rstrip().endswith("(") and l[m.end():].lstrip().startswith(")"):
                continue
            if re.match(r"\s*(=[^=]|\+=|-=|\+\+|--|\*=|\|=|&=)", l[m.end():]) or l[:m.start()].rstrip().endswith("&"):
                continue
            yield "paren@%d:%d" % (i, m.start()), lines[:i] + [l[:m.start()] + "(" + m.group(1) + ")" + l[m.end():]] + lines[i + 1:]


def op_init(lines, rng):
    """an initialiser at a top declaration as the first statement"""
    first, start = body_start(lines)
    for i in range(first, start):
        m = re.match(r"^(\s+)((?:(?:struct|union|unsigned|signed|const|static)\s+)*[A-Za-z_]\w*[\s\*]+)([A-Za-z_]\w*) = ([^,;]+);(.*)$", lines[i])
        if m:
            p, ty, v, e, rest = m.groups()
            new = lines[:i] + ["%s%s%s;%s" % (p, ty, v, rest)] + lines[i + 1:]
            new.insert(start, "    %s = %s;" % (v, e))
            yield "init@%d" % i, new


OPS = {"swap": op_swap, "move": op_move, "local": op_local, "inline": op_inline, "inexpr": op_assign_in_expr,
       "arms": op_arms, "ifelse": op_ifelse, "commute": op_commute, "compound": op_compound, "paren": op_paren,
       "init": op_init}


def mutants(lines, ops, rng, width):
    """up to `width` mutants of one text, the operators taking turns so that a prolific one (move) does not crowd
    out the others"""
    pools = []
    for n in ops:
        try:
            pool = list(OPS[n](lines, rng))
        except Exception as e:                            # a line model that does not fit this text: skip the operator
            pool = []
        rng.shuffle(pool)
        if pool:
            pools.append(pool)
    out = []
    while pools and len(out) < width:
        for p in list(pools):
            out.append(p.pop())
            if not p:
                pools.remove(p)
            if len(out) >= width:
                break
    return out


ROW = re.compile(r"^\s+(\S+)\s+(byte|shape|draft|far)\s+(\d+)\s+(\d+)\s+([0-9a-f]{8})\s")


# dcp try's note on a row whose frame size differs from the original's: the distance is then counted with the stack
# offsets aligned (every [esp+X] shifted), so it is far smaller than the real difference and not comparable
FRAME = re.compile(r"; frame (-?0x[0-9a-f]+) vs (-?0x[0-9a-f]+) \(stack offsets aligned\)")


def measure(name, comment, variants):
    """[(label, lines)] -> {label: (ins, bytes, code, frame)}; frame is "" when the variant's frame has the
    original's size, else "OURS vs ORIGINAL"; a variant that does not compile is left out. A `= label` row (the
    same code as another row) carries no note: it takes the note of that code's row."""
    out = {}
    for at in range(0, len(variants), 400):
        chunk = variants[at:at + 400]
        with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False, encoding="latin-1", newline="\n", dir=OUT) as f:
            for label, lines in chunk:
                f.write("### %s\n%s\n%s\n\n" % (label, "\n".join(comment), "\n".join(lines)))
            path = f.name
        text = dcp("try", name, path)
        os.unlink(path)
        if "variants=" not in text:
            sys.stderr.write(text[:2000] + "\n")
            raise SystemExit("dcp try failed")
        rows = [(ROW.match(l), FRAME.search(l)) for l in text.split("\n")]
        bycode = {m.group(5): "%s vs %s" % f.groups() for m, f in rows if m and f}   # one code, one frame
        for m, f in rows:
            if m:
                out[m.group(1)] = (int(m.group(3)), int(m.group(4)), m.group(5), bycode.get(m.group(5), ""))
    return out


def key(b):
    """the rank of a text: a frame of another size than the original's last (its distance does not count the
    shifted stack offsets), then instructions, bytes, the path's length"""
    return (1 if b["score"][3] else 0, b["score"][0], b["score"][1], len(b["path"]))


def base_text(spec, tree):
    """--base FILE or FILE:LABEL -> the start lines. FILE is either the whole function's text or a dcp try variants
    file: then LABEL (default: its only variant) is taken, its <<< old === new >>> blocks applied to the tree's text,
    or its body used as the whole function. A leading comment line of a whole-function body is dropped."""
    m = re.match(r"^/([a-zA-Z])/", spec)                      # a Git-Bash path /c/... for Windows Python
    if m:
        spec = m.group(1) + ":/" + spec[3:]
    path, label = spec, None
    if not os.path.exists(path) and ":" in spec[2:]:
        path, label = spec.rsplit(":", 1)
    text = open(path, encoding="latin-1").read().replace("\r\n", "\n")
    if not text.strip():
        raise SystemExit("--base %s is empty" % path)
    if re.search(r"^### ", text, re.M):
        parts = re.split(r"^### +(\S+)[^\n]*\n", text, flags=re.M)[1:]
        found = dict(zip(parts[0::2], parts[1::2]))
        if label is None:
            if len(found) != 1:
                raise SystemExit("--base %s has %d variants (%s): name one as FILE:LABEL" % (path, len(found), ", ".join(found)))
            label = next(iter(found))
        if label not in found:
            raise SystemExit("--base: no variant %r in %s (%s)" % (label, path, ", ".join(found)))
        text = found[label]
        blocks = re.findall(r"^\s*<<<\s*\n(.*?)\n\s*===\s*\n(.*?)\n?\s*>>>\s*$", text, re.M | re.S)
        if blocks:
            body = "\n".join(tree)
            for old, new in blocks:
                n = body.count(old)
                if n != 1:
                    raise SystemExit("--base: the old text of a block of %r occurs %d times in the function" % (label, n))
                body = body.replace(old, new)
            return body.split("\n")
    lines = [l.rstrip("\r") for l in text.strip("\n").split("\n")]
    if lines and lines[0].lstrip().startswith("/*") and lines[0].rstrip().endswith("*/"):
        lines = lines[1:]
    if not any(l.strip() for l in lines):
        raise SystemExit("--base %s gives an empty text" % spec)
    return lines


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("name")
    ap.add_argument("--rounds", type=int, default=30)
    ap.add_argument("--beam", type=int, default=8, help="texts kept per round")
    ap.add_argument("--width", type=int, default=120, help="mutants per kept text and round")
    ap.add_argument("--ops", default=",".join(OPS))
    ap.add_argument("--base", help="the start text: a file with the whole function, or a dcp try variants file as FILE:LABEL (blocks applied to the tree)")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--unsafe", action="store_true", help="move and swap without the meaning check (a statement may then cross a call, an exit or a statement it depends on)")
    ap.add_argument("--types", help="the local operator's types, comma separated (default int,float; e.g. int,float,unsigned int,short,char *)")
    ap.add_argument("--resume", action="store_true")
    a = ap.parse_args()
    global UNSAFE
    UNSAFE = a.unsafe
    if a.types:
        TYPES[:] = [t.strip() for t in a.types.split(",") if t.strip()]
    os.makedirs(OUT, exist_ok=True)
    rng = random.Random(a.seed)
    ops = [o for o in a.ops.split(",") if o in OPS]
    comment, tree = function_of(a.name)
    while tree and not tree[0].strip():
        tree.pop(0)
    start = tree
    if a.base:
        start = base_text(a.base, tree)
    state = os.path.join(OUT, a.name + ".json")
    beam = [dict(lines=start, path=[], score=None)]
    seen_text = set()
    if a.resume and os.path.exists(state):
        beam = json.load(open(state))["beam"]
    # a resumed beam is measured again (its scores may predate the frame column)
    base = measure(a.name, comment, [("b0", start)] + [("k%d" % i, b["lines"]) for i, b in enumerate(beam) if b["score"] is not None])
    if "b0" not in base:
        raise SystemExit("the start text does not compile")
    kept = []
    for i, b in enumerate(beam):
        if b["score"] is None or "k%d" % i in base:
            b["score"] = list(base["b0"] if b["score"] is None else base["k%d" % i])
            kept.append(b)
    beam = kept
    # a start whose frame has the original's size keeps it: a mutant with another frame size is dropped (its
    # aligned distance looks closer than it is); from a start with another size such mutants rank after every
    # mutant that reaches the original's size (key)
    keep_frame = not base["b0"][3]
    print("%s: start %d ins / %d bytes%s (the tree: %s), ops %s" % (a.name, base["b0"][0], base["b0"][1],
          "" if keep_frame else ", frame %s (offsets aligned: not comparable)" % base["b0"][3],
          "%d ins / %d bytes" % base["control"][:2] if "control" in base else "?", ",".join(ops)))
    beam = [b for b in beam if not (keep_frame and b["score"][3])] or [dict(lines=start, path=[], score=list(base["b0"]))]
    best = min(beam, key=key)
    for rnd in range(1, a.rounds + 1):
        if best["score"][0] == 0 and best["score"][1] == 0:
            break
        cand = []
        for bi, b in enumerate(beam):
            for label, lines in mutants(b["lines"], ops, rng, a.width if rnd > 1 else 100000):   # round 1: every single mutation
                h = hashlib.md5("\n".join(lines).encode()).hexdigest()
                if h in seen_text:
                    continue
                seen_text.add(h)
                cand.append(("r%db%dm%d" % (rnd, bi, len(cand)), lines, b["path"] + [label]))
        if not cand:
            print("round %d: no new mutants" % rnd)
            break
        res = measure(a.name, comment, [(l, t) for l, t, _ in cand])
        scored = [dict(lines=t, path=p, score=list(res[l])) for l, t, p in cand if l in res]
        framed = sum(1 for b in scored if b["score"][3])
        if keep_frame:
            scored = [b for b in scored if not b["score"][3]]
        pool = beam + scored
        pool.sort(key=key)
        # one text per compiled code, the best first; a few of the rest for breadth
        new, codes = [], set()
        for b in pool:
            if b["score"][2] in codes:
                continue
            codes.add(b["score"][2])
            new.append(b)
        keep = new[:max(1, a.beam - 2)]
        rest = new[len(keep):len(keep) + 40]
        rng.shuffle(rest)
        beam = keep + rest[:a.beam - len(keep)]
        best = beam[0]
        print("round %d: %d mutants, %d compiled, %d codes, %d with another frame size%s; best %d ins / %d bytes%s via %s" % (
            rnd, len(cand), len([l for l, _, _ in cand if l in res]), len(codes), framed, " (dropped)" if keep_frame else "",
            best["score"][0], best["score"][1], " frame " + best["score"][3] if best["score"][3] else "",
            " > ".join(best["path"][-4:]) or "(start)"))
        sys.stdout.flush()
        json.dump(dict(beam=beam), open(state, "w"))
        open(os.path.join(OUT, a.name + ".best.c"), "w", encoding="latin-1", newline="\n").write("\n".join(comment + best["lines"]) + "\n")   # with the comment: usable as a whole-function variant
    print("\nbest: %d ins / %d bytes%s%s, path: %s" % (best["score"][0], best["score"][1], "  BYTE" if best["score"][:2] == [0, 0] else "",
          "  frame %s (offsets aligned: not comparable)" % best["score"][3] if best["score"][3] else "", " > ".join(best["path"]) or "(start)"))
    print("text: %s" % os.path.relpath(os.path.join(OUT, a.name + ".best.c"), ROOT))
    # the diff against the start text (the --base variant, else the tree): printed when short, always in NAME.diff
    diff = list(difflib.unified_diff(start, best["lines"], "base" if a.base else "tree", "best", lineterm="", n=1))
    dpath = os.path.join(OUT, a.name + ".diff")
    open(dpath, "w", encoding="latin-1", newline="\n").write("\n".join(diff) + "\n")
    if len(diff) <= 40:
        print("\n".join(diff))
    else:
        print("diff: %d lines against the %s in %s" % (len(diff), "base" if a.base else "tree", os.path.relpath(dpath, ROOT)))


if __name__ == "__main__":
    main()
