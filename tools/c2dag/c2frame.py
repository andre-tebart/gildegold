r"""How VC6 lays out one function's stack frame, and why (C2 13.00.9044, stack.c).

    python tools/c2dag/c2frame.py src/main.c main_ChooseCity          # the symbol list and the slot table
    python tools/c2dag/c2frame.py src/main.c main_ChooseCity --refs   # plus every counted reference, in walk order
    python tools/c2dag/c2frame.py src/main.c FUNC --variants FILE [--label L]   # one dcp try variant
    python tools/c2dag/c2frame.py src/f3gm.c gm_CreateCharacter --names     # source names, conflicts, the packing

--variants reads dcp try's format exactly (c2trace.parse_variants): `<<<` old `===` new `>>>` blocks (markers may
be indented) replace text inside the function only; a body without blocks is the function's whole new text.

Rows:
  L  the locals at layout time, in list order: name, size, count
  B  the slot table before the sort: size, weight (sum of the members' counts), dens = weight * 1000 / size, members
  A  the slot table after the sort; index 0 is the lowest address (nearest esp); fixed parameter slots come first
  T / R (--refs)  every tuple with its operands and source line, and each counted reference in walk order
--names (every local and compiler temporary named by the source: a local by its name, `name@LINE` when two share
it, a temporary `tmp@LINE` = the first source line that uses it, `.2` for a second one there):
  N  the L rows (list order): name, size, count, kind, the source lines that use it (walk tuples), the first one
  B / A as above with these names
  X  the lifetime conflicts of each slot member: the locals live at the same time, which never share a slot (the
     packer's own interference sets, made symmetric as the packer tests them)
  P  the packing replayed: in list order each local joins or opens a slot, with every newer slot it passed and why
     (the members it conflicts with, or its size); a `?` means the replay disagrees with the B rows

The rule (read from c2.dll and confirmed by experiment):
  * count: one per operand that names the local, over all blocks, NO loop weighting. It is taken after global
    optimisation, register allocation and instruction selection but BEFORE the late passes (tail merging /
    cross-jumping, the late store-to-load peephole). So it is not the listing's count: a statement written in
    both arms of an if (merged late) counts twice; a reload that the late peephole removes counts; a lea that
    is later folded into several memory operands counts once.
  * list: size ascending (the declared size: 0xfc and 0x100 are different groups), then count descending; on a
    tie the local that reached the count FIRST in the walk stays ahead.
  * slots: each local in list order joins the first lifetime-compatible slot (searched from the last slot
    backward; the local at most twice the slot's size, the slot grows to the larger), else opens a new one. Scalars and arrays whose address never escaped merge; an array passed to a
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
from c2trace import variant_text, function_span  # the same variants reader (dcp try's format)


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
    ap.add_argument("--names", action="store_true", help="name every local and temporary by the source (tmp@LINE: the "
                    "first line that uses it) and print the lifetime conflicts (X) and the packing replayed (P)")
    a = ap.parse_args()
    setup()
    text = open(a.src, encoding="latin-1", newline="").read().replace("\r\n", "\n")
    tag = os.path.splitext(os.path.basename(a.src))[0]
    if a.variants:
        text, lab = variant_text(text, a.func, a.variants, a.label, src=a.src)
        tag += "_" + re.sub(r"\W", "_", lab)
    c = os.path.join(WORK, tag + ".c")
    frm = os.path.join(WORK, tag + ".frm")
    with open(c, "w", encoding="latin-1", newline="\n") as f:
        f.write(text)
    e = env()
    e["C2FRM_OUT"] = frm
    e["C2FRM_REFS"] = "1"
    e["C2FRM_TUP"] = "1"
    if a.names:
        e["C2FRM_IDS"] = "1"
    r = subprocess.run([os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/I" + os.path.join(ROOT, "include"),
                        "/I" + os.path.join(ROOT, "src"), "/I" + DX, "/Fa" + os.path.join(WORK, tag + ".asm"),
                        "/Fo" + os.path.join(WORK, tag + ".obj"), c], env=e, cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        raise SystemExit("compile failed:\n" + "\n".join(l for l in r.stdout.splitlines() if "error" in l))
    for b in open(frm, encoding="latin-1").read().split("WALK\n")[1:]:
        if re.search(r"(?m)^FUNC [_@]?%s(@\d+)? " % re.escape(a.func), b):
            if a.names:
                print("\n".join(named(b, text, a.func, a.refs)))
            else:
                print("\n".join(l for l in b.splitlines() if a.refs or not l.startswith(("R ", "T ", "X "))))
            return
    raise SystemExit("function not found in the trace (its frame was not packed: an unoptimised or frameless function)")


def named(block, text, func, refs):
    """the trace of one function with every symbol named by the source (--names): see the module doc"""
    lines = text.split("\n")
    decl = function_span(lines, func)[1] + 1           # the tuples' line numbers count from the declarator's line
    uses, order, info, conf, slots = {}, [], {}, {}, {"B": [], "A": []}
    tup = re.compile(r"\b[ud]\d+:(\S+)")
    for l in block.splitlines():
        if l.startswith("T  op"):
            m = re.search(r" line (-?\d+)$", l)
            for n in tup.findall(l):
                if m and int(m.group(1)) > 0:
                    uses.setdefault(n, set()).add(decl + int(m.group(1)))
        elif l.startswith("L "):
            m = re.match(r"L (\d+) (\S+) size (\d+) count (\d+) kind (\d+)", l)
            order.append(m.group(2))
            info[m.group(2)] = dict(size=int(m.group(3)), count=int(m.group(4)), kind=int(m.group(5)))
        elif l.startswith("X "):
            n, _, rest = l[2:].partition(" :")
            conf.setdefault(n, set()).update(rest.split())
        elif l[:2] in ("B ", "A "):
            m = re.match(r"([BA]) (\d+) size (\d+) weight (\d+) dens (\d+) align (\d+) :(.*)$", l)
            slots[m.group(1)].append(dict(size=int(m.group(3)), weight=int(m.group(4)), dens=int(m.group(5)), members=m.group(7).split()))
    # conflicts are tested both ways by the packer (the slot's union of its members' sets holds the local, or the
    # local's set holds a member), so they are symmetric here
    for n in list(conf):
        for o in conf[n]:
            conf.setdefault(o, set()).add(n)
    # the names: a named local by its name (with @line when two locals share it), a temporary as tmp@line
    def first(n):
        return min(uses[n]) if uses.get(n) else None
    base = {n: ("tmp" if info[n]["kind"] == 3 or n.startswith(("t", "#")) else re.sub(r"^_", "", n.split("@")[0])) for n in order}
    disp, taken = {}, set()
    for n in order:
        d = base[n]
        if d == "tmp" or sum(1 for o in order if base[o] == d) > 1:
            d += "@%s" % (first(n) or "?")
        k, d0 = 2, d
        while d in taken:
            d, k = "%s.%d" % (d0, k), k + 1
        taken.add(d)
        disp[n] = d
    def nm(n):
        return disp.get(n, re.sub(r"^_", "", n.split("@")[0]))
    def src(ln):
        return lines[ln - 1].strip()[:90] if ln and 0 < ln <= len(lines) else ""
    inslot = {m for s in slots["B"] for m in s["members"]}
    out = [next(l for l in block.splitlines() if l.startswith("FUNC "))]
    w = max([len(disp[n]) for n in order] + [4])
    for n in order:
        i = info[n]
        ls = sorted(uses.get(n, ()))
        out.append("N %-*s size %d count %d %s lines %s%s" % (w, disp[n], i["size"], i["count"], "param" if i["kind"] == 5 else "temp" if i["kind"] == 3 else "local",
                   " ".join(map(str, ls[:12])) + (" .." if len(ls) > 12 else "") if ls else "-", ("  | %d: %s" % (ls[0], src(ls[0]))) if ls else ""))
    for tag in ("B", "A"):
        for k, s in enumerate(slots[tag]):
            out.append("%s %d size %d weight %d dens %d : %s" % (tag, k, s["size"], s["weight"], s["dens"], " ".join(nm(m) for m in s["members"])))
    # conflicts: the locals that are live together and so never share a slot (parameters only when they own a slot)
    shown = [n for n in order if n in inslot]
    for n in shown:
        c = [nm(o) for o in shown if o != n and o in conf.get(n, ())]
        out.append("X %-*s : %s" % (w, disp[n], " ".join(c) or "-"))
    # the packing replayed: each local in list order joins the first slot, searched from the newest one back, that
    # holds no local it conflicts with and is at least half its size; else it opens a slot
    where = {m: k for k, s in enumerate(slots["B"]) for m in s["members"]}
    opened, members, size = [], {}, {}
    for n in order:
        if n not in where:
            continue
        k = where[n]
        skipped = []
        for j in reversed(opened):
            if j == k:
                break
            why = [nm(m) for m in members[j] if m in conf.get(n, ())]
            if why:
                skipped.append("%d (%s)" % (j, " ".join(why)))
            elif info[n]["size"] > 2 * size[j]:
                skipped.append("%d (size %d > 2 * %d)" % (j, info[n]["size"], size[j]))
            else:
                skipped.append("%d (? no conflict seen)" % j)
        if k in opened:
            bad = [nm(m) for m in members[k] if m in conf.get(n, ())]
            act = "joins %d" % k + (" (? conflicts %s)" % " ".join(bad) if bad else "")
        else:
            opened.append(k)
            members[k], size[k] = [], 0
            act = "opens %d" % k
        members[k].append(n)
        size[k] = max(size[k], info[n]["size"])
        out.append("P %-*s %s%s" % (w, disp[n], act, ("; passed " + ", ".join(skipped)) if skipped else ""))
    if refs:
        for l in block.splitlines():
            if l.startswith(("T ", "R ")):
                l = re.sub(r" line (\d+)$", lambda m: " line %d" % (decl + int(m.group(1))) if int(m.group(1)) > 0 else m.group(0), l)
                out.append(re.sub(r"(?<=[ :])([_#t][\w#]*(?:@[0-9a-f]+)?)(?=\s|$)", lambda m: nm(m.group(1)) if m.group(1) in info else m.group(1), l))
    return out


if __name__ == "__main__":
    main()
