"""permute: a dcp try variants file with every order of a run of statements (store fills, clears, inits, initialised
declarations), so nobody has to write the generator again.

    python tools/search/permute.py NAME A B [-o FILE] [--max N] [--sample S]
    dcp try NAME FILE

A and B are file lines (as `dcp fn NAME` prints them, at=src/unit.c:FIRST-LAST) of the run, inclusive; each line is one
statement and they are permuted as whole lines (a multi-line statement: join it first, or permute around it). The
variants are whole-function texts with the function's comment, labelled by the new order (`p3-0-1-2`; the identity is
left out). Up to --max permutations (default 5040 = 7 lines); with more, --sample S random orders (seeded) instead.
Only statements that do not depend on each other keep the meaning in every order: read the run first (climb.py's
move/swap check is stricter, permute.py does not check).
"""
import argparse, itertools, math, os, random, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("name")
    ap.add_argument("a", type=int)
    ap.add_argument("b", type=int)
    ap.add_argument("-o", "--out")
    ap.add_argument("--max", type=int, default=5040)
    ap.add_argument("--sample", type=int, default=0)
    ap.add_argument("--seed", type=int, default=1)
    a = ap.parse_args()
    r = subprocess.run(["dcp", "fn", a.name], cwd=ROOT, capture_output=True, text=True, encoding="latin-1")
    m = re.search(r"at=(\S+):(\d+)-(\d+)", r.stdout)
    if not m:
        sys.exit("permute: dcp fn %s gave no location" % a.name)
    path, first, last = os.path.join(ROOT, m.group(1)), int(m.group(2)), int(m.group(3))
    if not (first <= a.a < a.b <= last):
        sys.exit("permute: lines %d-%d are not inside %s (%d-%d)" % (a.a, a.b, a.name, first, last))
    lines = open(path, encoding="latin-1", newline="").read().replace("\r\n", "\n").split("\n")
    func = lines[first - 1:last]
    lo, hi = a.a - first, a.b - first
    run = func[lo:hi + 1]
    n = len(run)
    if any(not l.strip() for l in run):
        sys.exit("permute: the run has empty lines; give a run of statements only")
    total = math.factorial(n)
    if total <= a.max + 1:
        orders = [p for p in itertools.permutations(range(n)) if list(p) != list(range(n))]
    else:
        if not a.sample:
            sys.exit("permute: %d lines give %d orders (> --max %d): pass --sample S" % (n, total, a.max))
        rng = random.Random(a.seed)
        seen, orders = set(), []
        while len(orders) < min(a.sample, total - 1):
            p = list(range(n))
            rng.shuffle(p)
            if p != list(range(n)) and tuple(p) not in seen:
                seen.add(tuple(p))
                orders.append(p)
    out = a.out or os.path.join(ROOT, "build", "climb", "%s.perm%d-%d.txt" % (a.name, a.a, a.b))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w", encoding="latin-1", newline="\n") as f:
        for p in orders:
            body = func[:lo] + [run[i] for i in p] + func[hi + 1:]
            f.write("### p%s\n%s\n\n" % ("-".join(map(str, p)), "\n".join(body)))
    print("%d variants (%d lines) -> %s" % (len(orders), n, os.path.relpath(out, ROOT)))
    print("then: dcp try %s %s" % (a.name, out.replace("\\", "/")))


if __name__ == "__main__":
    main()
