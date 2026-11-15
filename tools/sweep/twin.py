"""Twins: for a below-byte function, the byte functions whose source is nearly the same, with the lines that differ.

    python tools/sweep/twin.py NAME [NAME ..]     # the twins of these functions
    python tools/sweep/twin.py --stuck            # every @stuck / @deep function that has a byte twin
    python tools/sweep/twin.py NAME --min 0.4     # a lower similarity bound (default 0.5)
    python tools/sweep/twin.py NAME --all         # twins of any grade, not only byte ones

Why: the game has many copied functions (main_ChoosePlayer / lobby_ChoosePlayer, the sv_ / sv_t pairs, the
per-profession ai_ handlers). When one copy reaches byte the lever that did it (one array reused, a block local, a
statement order) is the other copy's lever too, but the stuck note of the other copy still says "the twin has the
same problem". main_ChoosePlayer sat stuck with 36 slot lines for that reason: lobby_ChoosePlayer had been
matched with `int row[4]` for both button rows, and that one declaration was the whole difference.

Run this FIRST on every deep item, before any variant: a byte twin's differing lines are the variants to try.
The similarity is the Jaccard index of the normalised statement lines (identifiers of locals replaced by their
order of first use, so renamed locals still compare equal); the diff printed is over those normalised lines, with
the twin's real text for the lines only it has.
"""
import argparse, csv, difflib, glob, os, re, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
START = re.compile(r"^/\* (0x[0-9a-f]+),")
KEYWORDS = set("if else for while do return break continue switch case default goto sizeof int char short long "
               "unsigned signed float double void struct union const static".split())


def grades():
    g = {}
    with open(os.path.join(ROOT, "ledger", "grades.tsv"), encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            g[int(row["address"], 16)] = (row["name"], row["unit"], row["code"])
    return g


def functions():
    """address -> (file, first line, the body's lines, the comment's text)"""
    out = {}
    for path in sorted(glob.glob(os.path.join(ROOT, "src", "*.c"))):
        lines = open(path, encoding="latin-1").read().split("\n")
        i = 0
        while i < len(lines):
            m = START.match(lines[i])
            if not m:
                i += 1
                continue
            j = i
            while j < len(lines) and "*/" not in lines[j]:
                j += 1
            k = j + 1
            while k < len(lines) and lines[k].rstrip("\r") != "}":
                if START.match(lines[k]):
                    break
                k += 1
            if k < len(lines) and lines[k].rstrip("\r") == "}":
                out[int(m.group(1), 16)] = (os.path.relpath(path, ROOT), i + 1,
                                            [l.rstrip("\r") for l in lines[j + 1:k + 1]], "\n".join(lines[i:j + 1]))
                i = k + 1
            else:
                i = k
    return out


def locals_of(body):
    names = []
    depth = 0
    for l in body:
        if depth >= 1:
            m = re.match(r"^\s+(?:(?:struct|union|unsigned|signed|const|static)\s+)*[A-Za-z_]\w*[\s\*]+(.*);\s*(/\*.*\*/)?\s*$", l)
            if m and "(" not in m.group(1).split("=")[0] and not re.match(r"^\s+(return|goto|else)\b", l):
                for d in m.group(1).split(","):
                    n = re.match(r"[\s\*]*(\w+)", d)
                    if n and n.group(1) not in KEYWORDS:
                        names.append(n.group(1))
        depth += l.count("{") - l.count("}")
    return set(names)


def normal(body):
    """(normalised line, real line) per statement line: locals as v1, v2 .. in order of first use; declarations,
    comments and blank lines dropped"""
    loc = locals_of(body)
    order = {}
    out = []
    comment = False
    depth = 0
    for l in body:
        d0 = depth
        depth += l.count("{") - l.count("}")
        s = l
        if comment:
            if "*/" not in s:
                continue
            s = s.split("*/", 1)[1]
            comment = False
        s = re.sub(r"/\*.*?\*/", "", s)
        if "/*" in s:
            s = s.split("/*", 1)[0]
            comment = True
        s = s.strip()
        if not s or d0 == 0:
            continue
        if re.match(r"^(?:(?:struct|union|unsigned|signed|const|static)\s+)*[A-Za-z_]\w*[\s\*]+[\w\s,\*\[\]]+;$", s) \
                and not re.match(r"^(return|goto|break|continue)\b", s):
            continue

        def sub(m):
            w = m.group(0)
            if w in loc:
                return order.setdefault(w, "v%d" % (len(order) + 1))
            return w
        out.append((re.sub(r"[A-Za-z_]\w*", sub, re.sub(r"\s+", " ", s)), l))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("names", nargs="*")
    ap.add_argument("--stuck", action="store_true", help="every @stuck / @deep function")
    ap.add_argument("--min", type=float, default=0.5)
    ap.add_argument("--all", action="store_true", help="twins of any grade")
    ap.add_argument("--brief", action="store_true", help="no diff, one line per twin")
    a = ap.parse_args()
    g = grades()
    fns = functions()
    by_name = {g[ad][0]: ad for ad in fns if ad in g}
    norm = {ad: normal(f[2]) for ad, f in fns.items()}
    sets = {ad: set(n for n, _ in v) for ad, v in norm.items()}
    targets = list(a.names)
    if a.stuck:
        targets += [g[ad][0] for ad, f in sorted(fns.items()) if ad in g and g[ad][2] != "byte"
                    and re.search(r"@(stuck|deep)\b", f[3])]
    for name in targets:
        if name not in by_name:
            print("%s: not found in src/" % name)
            continue
        ad = by_name[name]
        mine = sets[ad]
        if len(mine) < 4:
            continue
        hits = []
        for o, s in sets.items():
            if o == ad or o not in g or (not a.all and g[o][2] != "byte") or len(s) < 4:
                continue
            j = len(mine & s) / float(len(mine | s))
            if j >= a.min:
                hits.append((j, o))
        hits.sort(reverse=True)
        if not hits:
            if not a.stuck:
                print("%s: no %stwin at %.2f" % (name, "" if a.all else "byte ", a.min))
            continue
        for j, o in hits[:3]:
            print("%s (%s, %s): twin %s (%s, %s) similarity %.2f  %s:%d" % (
                name, g[ad][1], g[ad][2], g[o][0], g[o][1], g[o][2], j, fns[o][0], fns[o][1]))
            if a.brief or a.stuck:
                continue
            x = [n for n, _ in norm[ad]]
            y = [n for n, _ in norm[o]]
            for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, x, y, autojunk=False).get_opcodes():
                if tag == "equal":
                    continue
                for k in range(i1, i2):
                    print("  - " + norm[ad][k][1].strip())
                for k in range(j1, j2):
                    print("  + " + norm[o][k][1].strip())
                print("  --")
            dm = [l.strip() for l in fns[ad][2][:40] if re.search(r"\[\w+\]", l) and l.strip().endswith(";") and "=" not in l]
            do = [l.strip() for l in fns[o][2][:40] if re.search(r"\[\w+\]", l) and l.strip().endswith(";") and "=" not in l]
            if dm != do:
                print("  array declarations: ours   " + " | ".join(dm))
                print("                      twin   " + " | ".join(do))


if __name__ == "__main__":
    sys.exit(main())
