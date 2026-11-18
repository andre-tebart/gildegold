"""gradecmp.py: every function whose current grade (dcp grade, per unit) differs from ledger/grades.tsv, the committed
baseline. Use after a header or data change: a `dcp try` variant cannot reproduce a tie-break ledger row, so a function
that was already below byte can look like a new regression; this compares against the baseline instead (data s11)."""
import subprocess, re
base = {}
for l in open(r"C:\Users\andre\Documents\gildegold\ledger\grades.tsv", encoding="utf-8"):
    p = l.rstrip("\n").split("\t")
    if len(p) > 4 and p[0] == "exe":
        base[p[2]] = (p[4], p[5])
out = subprocess.run("dcp grade", shell=True, capture_output=True, text=True, cwd=r"C:\Users\andre\Documents\gildegold").stdout
units = re.findall(r"unit=(\S+)\s+range", out)
for u in units:
    o = subprocess.run("dcp grade " + u, shell=True, capture_output=True, text=True, cwd=r"C:\Users\andre\Documents\gildegold").stdout
    for m in re.finditer(r"name=(\S+)\s+unit=\S+\s+experiment=(\S+)\s+readability=(\S+)", o):
        n, e, r = m.groups()
        if n in base and base[n] != (e, r):
            print(u, n, base[n], "->", (e, r))
