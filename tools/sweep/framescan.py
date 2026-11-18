"""framescan.py: the frame-sweep candidate list (frame sweep s21).

    uv run python tools/sweep/framescan.py [--out FILE] [NAME ..]

For every function below byte (ledger/grades.tsv) runs `dcp frame NAME` and keeps those whose frame size differs
from the original's or that have `shared in the original:` lines, AND whose source has a function-scope local whose
address escapes (`&x` anywhere, or an array / record local named as a call argument). Those locals are the lever of
vc6-matching "Size queue s20": an escaped function-scope local gets a slot that conflicts with everything; block
locals with disjoint scopes share one. Ranked: in the size queue first, then shape before draft, then fewest
differing instructions (from `dcp grade NAME`).
"""
import argparse, os, re, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
DECL = re.compile(r"^    ((?:struct |union |unsigned |signed |const )*[A-Za-z_]\w*(?:\s*\*+\s*|\s+))(\w+)((?:\[[^\]]*\])*)\s*(?:=[^;]*)?;")


def run(cmd):
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    return r.stdout + r.stderr


def below_byte():
    out = []
    for l in open(os.path.join(ROOT, "ledger", "grades.tsv"), encoding="utf-8"):
        p = l.rstrip("\n").split("\t")
        if len(p) > 4 and p[0] == "exe" and p[4] != "byte":
            out.append((p[2], p[3], p[4]))
    return out


def size_queue():
    q = set()
    for l in run(["uv", "run", "python", "tools/exe/exemeasure.py", "--queue", "-n", "400"]).splitlines():
        for m in re.finditer(r"\b([a-z0-9]+_\w+)\b", l):
            q.add(m.group(1))
    return q


def escaped_locals(name):
    head = run(["dcp", "fn", name]).splitlines()
    if not head:
        return []
    m = re.search(r"at=(\S+):(\d+)-(\d+)", head[0])
    if not m:
        return []
    lines = open(os.path.join(ROOT, m.group(1)), encoding="latin-1").read().split("\n")[int(m.group(2)) - 1:int(m.group(3))]
    body = next((i for i, l in enumerate(lines) if l.rstrip() == "{"), None)
    if body is None:
        return []
    locs = []
    i = body + 1
    while i < len(lines):
        l = lines[i].rstrip()
        if not l.strip():
            i += 1
            continue
        dm = DECL.match(l)
        if not dm:
            break
        for part in re.split(r",\s*", l.strip().rstrip(";")):
            pm = re.search(r"(\w+)\s*((?:\[[^\]]*\])*)\s*(=.*)?$", part)
            if pm:
                locs.append((pm.group(1), bool(pm.group(2)), dm.group(1).strip()))
        i += 1
    text = "\n".join(lines[i:])
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    esc = []
    for n, arr, typ in locs:
        if re.search(r"&\s*%s\b(?!\s*->)" % re.escape(n), text):
            esc.append(n)
        elif arr and re.search(r"[(,]\s*%s\s*[,)]" % re.escape(n), text):
            esc.append(n)
    return esc


def vector_locals(name):
    head = run(["dcp", "fn", name]).splitlines()
    m = re.search(r"at=(\S+):(\d+)-(\d+)", head[0]) if head else None
    if not m:
        return []
    lines = open(os.path.join(ROOT, m.group(1)), encoding="latin-1").read().split("\n")[int(m.group(2)) - 1:int(m.group(3))]
    out = []
    for l in lines:
        dm = re.match(r"^\s+D3VECTOR\s+([^;(]+);", l)
        if dm:
            out += [v.strip().lstrip("*") for v in dm.group(1).split(",") if "*" not in v]
    return out


def ins_of(name):
    o = run(["dcp", "grade", name])
    m = re.search(r"dist='(\d+) ins", o)
    return int(m.group(1)) if m else 9999


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("names", nargs="*")
    ap.add_argument("--out")
    ap.add_argument("--vec", action="store_true", help="instead: ours smaller than the original's with a function-scope D3VECTOR local (the original's D3VECTOR4, 4 bytes more per vector)")
    a = ap.parse_args()
    fns = below_byte()
    if a.names:
        fns = [f for f in fns if f[0] in a.names]
    sq = size_queue()
    rows = []
    for name, unit, grade in fns:
        fr = run(["dcp", "frame", name])
        m = re.search(r"frame: ours (0x[0-9a-f]+) bytes.*?the original (0x[0-9a-f]+)", fr)
        if not m:
            continue
        ours, orig = int(m.group(1), 16), int(m.group(2), 16)
        shared = [l.strip() for l in fr.splitlines() if "shared in the original:" in l]
        if a.vec:
            if ours >= orig:
                continue
            esc = vector_locals(name)
        elif ours == orig and not shared:
            continue
        else:
            esc = escaped_locals(name)
        if not esc:
            continue
        rows.append([name in sq, grade, 0, name, unit, ours, orig, len(shared), esc, shared])
        sys.stderr.write("%s %s\n" % (name, esc))
    for r in rows:
        r[2] = ins_of(r[3])
    rows.sort(key=lambda r: (not r[0], r[1] != "shape", r[2]))
    out = []
    for r in rows:
        out.append("%-30s %-8s sq=%d %-5s ins=%-5d frame %#x/%#x shared=%d esc=%s" % (
            r[3], r[4], r[0], r[1], r[2], r[5], r[6], r[7], ",".join(r[8])))
        for s in r[9]:
            out.append("    " + s)
    txt = "\n".join(out)
    print(txt)
    if a.out:
        open(a.out, "w", encoding="utf-8").write(txt + "\n")


if __name__ == "__main__":
    main()
