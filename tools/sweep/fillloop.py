"""Sweep: a constant memset fill written as the plain element loop.

    python tools/sweep/fillloop.py                 # every below-byte function with a memset(.., 0xff / -1, ..)
    python tools/sweep/fillloop.py --zero          # the zero fills too (many more functions)
    python tools/sweep/fillloop.py NAME [NAME ..]  # only these functions
    python tools/sweep/fillloop.py --apply         # write the byte hits into the sources (probe spelling, see below)

Why: VC6 /O2 compiles `for (i = 0; i < 8; i++) a[i] = -1;` to the same `rep stosd` as `memset(a, 0xff, 32)`,
but not to the same code around it: for the loop it computes the address into a scratch temp and copies it to
edi (`lea edx,[edx+X]; mov edi,edx`), for the memset it targets edi (`lea edi,[edx+X]`), and the neighbouring
stores are scheduled differently. ahm_CreateAlchemist (3 ins, stuck after 359 statement orders) was byte with the
loop. This script tries that respelling on every memset of every function that is below byte and prints the
functions it moves.

Each `memset(DST, V, SIZE);` statement of a function becomes the probe
    for (fi_ = 0; fi_ < (int)(SIZE) / (int)sizeof(T); fi_++) ((T *)(DST))[fi_] = V;
with T = int (and short / char as further variants with --widths), one variant per memset and one with all of
them, measured with `dcp try`. The probe spelling is what --apply writes: clean it up by hand afterwards (the
function's own index variable, the array's own element type and count) and check that it is still byte.
"""
import argparse, os, re, subprocess, sys, tempfile

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
MEMSET = re.compile(r"^([ \t]*)memset\((.*),\s*(0xff|-1|0xffffffff|0)\s*,\s*([^;]*)\);[ \t]*(/\*.*\*/)?[ \t]*$", re.M)


def dcp(*args):
    r = subprocess.run(["dcp"] + list(args), cwd=ROOT, capture_output=True, text=True, encoding="utf-8",
                       errors="replace")
    return r.stdout + r.stderr


def queue():
    """(name, unit, dist) of every function below byte: the deep and the base queue"""
    out = []
    for q in ("--deep", "--base"):
        for line in dcp("next", q, "-n", "5000").splitlines():
            m = re.search(r"name=(\S+)\s+unit=(\S+).*?dist='([^']*)'.*?file=(\S+)", line)
            if m and m.group(1) not in [o[0] for o in out]:
                out.append((m.group(1), m.group(4), m.group(3)))
    return out


def function_text(name):
    """the function's lines in its file (address comment included), as dcp fn locates them"""
    head = dcp("fn", name).splitlines()[0]
    m = re.search(r"at=(\S+):(\d+)-(\d+)", head)
    if not m:
        return None
    lines = open(os.path.join(ROOT, m.group(1)), encoding="latin-1").read().split("\n")
    return "\n".join(l.rstrip("\r") for l in lines[int(m.group(2)) - 1:int(m.group(3))])


def loop(indent, dst, val, size, width):
    v = "0" if val == "0" else "-1"
    return "%sfor (fi_ = 0; fi_ < (int)(%s) / (int)sizeof(%s); fi_++)\n%s    ((%s *)(%s))[fi_] = %s;" % (
        indent, size.strip(), width, indent, width, dst.strip(), v)


def variants(text, values, widths):
    hits = [m for m in MEMSET.finditer(text) if m.group(3) in values]
    if not hits:
        return {}
    # the declaration goes after the function's opening brace
    brace = re.search(r"^\{[ \t]*$", text, re.M)
    if not brace:
        return {}
    out = {}
    for w in widths:
        sets = [[h] for h in hits] + ([hits] if len(hits) > 1 else [])
        for k, chosen in enumerate(sets):
            t = text
            for h in sorted(chosen, key=lambda h: -h.start()):
                t = t[:h.start()] + loop(h.group(1), h.group(2), h.group(3), h.group(4), w) + t[h.end():]
            t = t[:brace.end()] + "\n    int fi_;" + t[brace.end():]
            label = "%s-%s" % (w.replace(" ", ""), "all" if len(chosen) > 1 else "m%d" % k)
            out[label] = t
    return out


def run(name, vs):
    with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False, encoding="latin-1", newline="\n") as f:
        for label, t in vs.items():
            f.write("### %s\n%s\n\n" % (label, t))
        path = f.name
    rows = {}
    for line in dcp("try", name, path).splitlines():
        m = re.match(r"\s+(\S+)\s+(byte|shape|draft|proved|error\S*)\s+(\d+)\s+(\d+)\s+(\S+)", line)
        if m:
            rows[m.group(1)] = (m.group(2), int(m.group(3)), int(m.group(4)))
    return rows, path


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("names", nargs="*")
    ap.add_argument("--zero", action="store_true", help="the zero fills too")
    ap.add_argument("--widths", default="int", help="element types to try, comma separated (int,short,char)")
    ap.add_argument("--apply", action="store_true", help="write each byte hit into the source (probe spelling)")
    a = ap.parse_args()
    values = {"0xff", "-1", "0xffffffff"} | ({"0"} if a.zero else set())
    todo = [q for q in queue() if not a.names or q[0] in a.names]
    tried = moved = 0
    for name, _file, dist in todo:
        text = function_text(name)
        if not text or "memset(" not in text:
            continue
        vs = variants(text, values, [w.strip() for w in a.widths.split(",")])
        if not vs:
            continue
        tried += 1
        rows, path = run(name, vs)
        ctl = rows.get("control")
        if not ctl:
            print("%-34s no control row (dcp try failed)" % name)
            continue
        best = min((r for r in rows.items() if r[0] != "control"), key=lambda r: (r[1][0] != "byte", r[1][1], r[1][2]),
                   default=None)
        if best and (best[1][0] == "byte" or best[1][1] < ctl[1]):
            moved += 1
            print("%-34s %s %d ins -> %s: %s %d ins   (dcp try %s %s --apply %s)" % (
                name, ctl[0], ctl[1], best[0], best[1][0], best[1][1], name, path, best[0]))
            if a.apply and best[1][0] == "byte":
                dcp("try", name, path, "--apply", best[0])
        else:
            os.unlink(path)
        sys.stdout.flush()
    print("functions with a fill: %d, moved closer: %d" % (tried, moved))


if __name__ == "__main__":
    main()
