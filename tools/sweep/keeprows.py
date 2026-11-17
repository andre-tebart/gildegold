"""keeprows.py NAME...: restore ledger/tiebreak.tsv from HEAD except the rows of the named functions.
Workaround for the known dcp bug (a build or `dcp grade` rewrites tiebreak rows of functions whose text did not
change): run it before committing, naming the functions you worked on, then commit only those rows.
  python tools/sweep/keeprows.py ai_Foo ai_Bar"""
import subprocess, sys
keep = set(sys.argv[1:])
head = subprocess.run(["git", "show", "HEAD:ledger/tiebreak.tsv"], capture_output=True, text=True, encoding="utf-8").stdout.split("\n")
cur = open("ledger/tiebreak.tsv", encoding="utf-8", newline="").read().split("\n")
curmap = {}
for l in cur:
    f = l.split("\t")
    if len(f) > 2: curmap[(f[0], f[1])] = l
out = []
for l in head:
    f = l.split("\t")
    if len(f) > 2 and f[2] in keep and (f[0], f[1]) in curmap:
        out.append(curmap[(f[0], f[1])])
    else:
        out.append(l)
open("ledger/tiebreak.tsv", "w", encoding="utf-8", newline="").write("\n".join(out))
