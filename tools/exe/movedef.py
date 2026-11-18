"""movedef: move data definitions (with their comment, also a comment over several lines) between units.

    uv run python tools/exe/movedef.py SYM TARGET_UNIT AFTER_SYM [SYM TARGET_UNIT AFTER_SYM ...]

Cuts SYM's definition out of whatever src/*.c defines it and inserts it into src/TARGET_UNIT.c after the definition
of AFTER_SYM ('^AFTER_SYM': before it). A definition line starts at column 0, is no extern / # / comment and holds
SYM followed by [..], = or ;. Refuses when SYM or AFTER_SYM is defined more than once or AFTER_SYM is not in the
target unit. An initialiser over several lines ({ .. };) is not followed: move those by hand.

Why: .bss / .data by place (notes/vc6-data.md): a datum lying inside another unit's block is that unit's; VC6
emits a unit's = 0 data in definition order, so the definition goes between its address neighbours (s10 moved
~45 data into sv / main / f3init / f3gmt / gs / lcmap this way). Rebuild and check `dcp status` "need" after.
Writes retry for a few seconds (the daemon or an editor may hold a file briefly).
"""
import glob, os, re, sys, time


def write(f, text):
    for n in range(20):
        try:
            with open(f, "w", encoding="latin-1", newline="") as h:
                h.write(text)
            return
        except OSError:
            time.sleep(0.5)
    raise SystemExit("cannot write " + f)
ROOT = os.path.join(os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")), "src") + os.sep


def find_def(sym):
    pat = re.compile(r"^(?!extern|#|/|\s)[^(]*?\b" + re.escape(sym) + r"\b\s*(\[[^\]]*\]\s*)*(=|;)")
    hits = []
    for f in glob.glob(ROOT + "*.c"):
        lines = open(f, encoding="latin-1", newline="").read().split("\n")
        for i, l in enumerate(lines):
            if pat.match(l):
                hits.append((f, i))
    if len(hits) != 1:
        sys.exit("definition of %s: %r" % (sym, hits))
    return hits[0]


def extent(lines, i):
    """the lines i..k-1 of the definition at i (its comment may go on over lines)"""
    k = i + 1
    t = lines[i]
    while t.count("/*") != t.count("*/"):
        t += "\n" + lines[k]
        k += 1
    return k


args = sys.argv[1:]
if not args or len(args) % 3:
    sys.exit(__doc__)
for k in range(0, len(args), 3):
    sym, unit, after = args[k:k + 3]
    f, i = find_def(sym)
    lines = open(f, encoding="latin-1", newline="").read().split("\n")
    e = extent(lines, i)
    block = lines[i:e]
    before = after.startswith("^")
    tf, j = find_def(after.lstrip("^"))
    if not tf.replace("\\", "/").endswith("/" + unit + ".c"):
        sys.exit("%s is not in %s.c (%s)" % (after, unit, tf))
    if tf == f:
        sys.exit("same file: " + sym)
    del lines[i:e]
    tl = open(tf, encoding="latin-1", newline="").read().split("\n")
    at = j if before else extent(tl, j)
    tl[at:at] = block
    write(tf, "\n".join(tl))
    write(f, "\n".join(lines))
    print("%s: %s -> %s.c %s %s" % (sym, f.replace("\\", "/").split("/")[-1], unit, "before" if before else "after", after.lstrip("^")))
