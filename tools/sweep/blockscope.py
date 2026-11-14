"""Sweep: a function-scope array / record that is only used inside blocks, declared in those blocks instead.

    python tools/sweep/blockscope.py                 # every below-byte function
    python tools/sweep/blockscope.py NAME [NAME ..]  # only these
    python tools/sweep/blockscope.py --unit main     # one unit's functions
    python tools/sweep/blockscope.py --scalars       # scalars too (default: arrays and struct / typedef'd records)

Why: VC6 packs block locals with disjoint scopes into one stack slot, so the frame size and the instructions do
not tell whether `AIMSG msg;` stood at the top of the function or in each block that fills it. Two other things
do: the scheduler orders a pointer load after every earlier store to an address-taken local whose address
already went to a call (a fresh block local has not escaped yet), and the frame sort sees one slot per group of
merged block locals at another table position. main_EndRound (AIMSG msg, nine blocks), main_ChooseCity (pos /
rot, key) and the 2026-11-10 sweep (9 byte) all matched with this change alone or as the last step.

For each candidate local the script removes the function-scope declaration and declares the local at the top of
every outermost block (a child block of the function body, or the deepest block inside it that still holds all
of that child's uses) that uses it; a local with a use directly in the function body is skipped. One variant per
local and one with all of them, measured with `dcp try`. Hits are printed with the command that applies them;
check the result reads well (the script inserts the declaration and a blank line after the block's brace).
"""
import argparse, os, re, subprocess, sys, tempfile

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
DECL = re.compile(r"^    ((?:struct |union |unsigned |signed |const )*[A-Za-z_]\w*(?: \*+| +\*+|\s+))(\w+)((?:\[[^\]]*\])*);[ \t]*(/\*.*\*/)?[ \t]*$")
SCALARS = {"int", "char", "short", "long", "float", "double", "unsigned", "signed"}


def dcp(*args):
    r = subprocess.run(["dcp"] + list(args), cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    return r.stdout + r.stderr


def queue(unit=None):
    out = []
    for q in ("--deep", "--base"):
        for line in dcp("next", q, "-n", "5000").splitlines():
            m = re.search(r"name=(\S+)\s+unit=(\S+)", line)
            if m and (not unit or m.group(2) == unit) and m.group(1) not in out:
                out.append(m.group(1))
    return out


def function_text(name):
    head = dcp("fn", name).splitlines()[0]
    m = re.search(r"at=(\S+):(\d+)-(\d+)", head)
    if not m:
        return None
    lines = open(os.path.join(ROOT, m.group(1)), encoding="latin-1").read().split("\n")
    return [l.rstrip("\r") for l in lines[int(m.group(2)) - 1:int(m.group(3))]]


def strip(line):
    """the line without string / char literals and comments (enough for brace counting and identifier search)"""
    line = re.sub(r'"(\\.|[^"\\])*"', '""', line)
    line = re.sub(r"'(\\.|[^'\\])*'", "''", line)
    return re.sub(r"/\*.*?\*/", "", line).split("//")[0]


def blocks(lines, body):
    """for each line index from `body` on: the chain of open blocks, each identified by the line of its `{`"""
    chain, out = [], {}
    for i in range(body, len(lines)):
        s = strip(lines[i])
        opened = []
        for ch in s:
            if ch == "{":
                chain.append(i); opened.append(i)
            elif ch == "}" and chain:
                chain.pop()
        # a line belongs to the blocks open after its own braces, except that the `{` line itself opens the block
        out[i] = list(chain)
    return out


def variants(lines, scalars):
    body = next((i for i, l in enumerate(lines) if l == "{"), None)
    if body is None:
        return {}
    chain = blocks(lines, body)
    cands = []
    i = body + 1
    while i < len(lines) and (DECL.match(lines[i]) or not lines[i].strip() or re.match(r"^    [\w \*]+[\w\[\], \*]*;", lines[i])):
        m = DECL.match(lines[i])
        if m:
            base = m.group(1).split()[0] if m.group(1).split() else ""
            pointer = "*" in m.group(1)
            record = not pointer and (bool(m.group(3)) or base not in SCALARS or m.group(1).startswith(("struct", "union")))
            if record or (scalars and not pointer):
                cands.append((i, m.group(2), lines[i].strip()))
        i += 1
    first_stmt = i
    out, all_plan = {}, []
    for di, name, decl in cands:
        uses = [k for k in range(first_stmt, len(lines)) if re.search(r"\b%s\b" % re.escape(name), strip(lines[k]))]
        if not uses:
            continue
        groups = {}
        ok = True
        for k in uses:
            c = [b for b in chain[k] if b != body]
            # a use on the line that opens a block (`if (x) {`) belongs to the enclosing block
            c = [b for b in c if b != k] if "{" in strip(lines[k]) else c
            if not c:
                ok = False
                break
            groups.setdefault(c[0], []).append(c)
        if not ok:
            continue
        places = []
        for top, chains in groups.items():
            # the deepest block common to all uses under this child block
            common = chains[0]
            for c in chains[1:]:
                n = 0
                while n < len(common) and n < len(c) and common[n] == c[n]:
                    n += 1
                common = common[:n]
            places.append(common[-1])
        plan = (di, name, decl, sorted(set(places)))
        all_plan.append(plan)
        out[name] = apply(lines, [plan])
    if len(all_plan) > 1:
        out["ALL"] = apply(lines, all_plan)
    return out


def apply(lines, plans):
    ins, drop = {}, set()
    for di, name, decl, places in plans:
        drop.add(di)
        for p in places:
            ins.setdefault(p, []).append(decl)
    res = []
    for i, l in enumerate(lines):
        if i in drop:
            continue
        res.append(l)
        if i in ins:
            indent = re.match(r"\s*", lines[i + 1] if i + 1 < len(lines) and lines[i + 1].strip() else l + "    ").group(0)
            if not (i + 1 < len(lines) and lines[i + 1].strip()):
                indent = re.match(r"\s*", l).group(0) + "    "
            # after declarations already at the block's top, else at the top followed by a blank line
            res.extend(indent + d for d in ins[i])
            nxt = lines[i + 1] if i + 1 < len(lines) else ""
            if not re.match(r"\s*((struct |union |unsigned |const )*[A-Za-z_]\w*[ \*]+\w+(\[[^\]]*\])*( = [^;]*)?;)", nxt):
                res.append("")
    return "\n".join(res)


def run(name, vs):
    with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False, encoding="latin-1", newline="\n") as f:
        for label, t in vs.items():
            f.write("### %s\n%s\n\n" % (label, t))
        path = f.name
    rows = {}
    for line in dcp("try", name, path).splitlines():
        m = re.match(r"\s+(\S+)\s+(byte|shape|draft|proved)\s+(\d+)\s+(\d+)\s+(\S+)", line)
        if m:
            rows[m.group(1)] = (m.group(2), int(m.group(3)), int(m.group(4)), m.group(5))
    return rows, path


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("names", nargs="*")
    ap.add_argument("--unit")
    ap.add_argument("--scalars", action="store_true")
    a = ap.parse_args()
    todo = a.names or queue(a.unit)
    tried = moved = 0
    for name in todo:
        lines = function_text(name)
        if not lines:
            continue
        try:
            vs = variants(lines, a.scalars)
        except Exception as e:  # a shape of code the brace reader does not handle
            print("%-34s skipped (%s)" % (name, e))
            continue
        if not vs:
            continue
        tried += 1
        rows, path = run(name, vs)
        ctl = rows.get("control")
        if not ctl or ctl[0] == "byte":
            os.unlink(path)
            continue
        better = sorted(((r[0] != "byte", r[1], r[2]), l, r) for l, r in rows.items()
                        if l != "control" and r[3] != ctl[3] and (r[0] == "byte" or r[1] < ctl[1]))
        if better:
            moved += 1
            _, lab, r = better[0]
            print("%-34s %s %d ins -> %s: %s %d ins   (dcp try %s %s --apply %s)" % (name, ctl[0], ctl[1], lab, r[0], r[1], name, path, lab))
        else:
            os.unlink(path)
        sys.stdout.flush()
    print("functions with a candidate local: %d, moved closer: %d" % (tried, moved))


if __name__ == "__main__":
    main()
