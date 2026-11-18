"""retgen.py FUNC mode [mask-list]: dcp try variants that turn the `return;` sites after a switch into `break;`,
mode `single`: all sites, each site alone, all but one; mode `mask 0+2,1+3`: the listed site sets (sites numbered
in source order, printed to stderr). VC6 lays out a switch exit differently for return and break, so the
original's per-case choice decides size and tails (size queue s13). Prints the variant file to stdout:
  python tools/search/retgen.py he_RunPlague single > build/v.txt && dcp try he_RunPlague build/v.txt"""
import subprocess, sys, re, itertools
name = sys.argv[1]; mode = sys.argv[2]
out = subprocess.run(["dcp", "fn", name], capture_output=True, text=True).stdout.splitlines()
at = re.search(r"at=(\S+):(\d+)-(\d+)", out[0])
path, a, b = at.group(1), int(at.group(2)), int(at.group(3))
lines = open(path, encoding="utf-8").read().split("\n")[a-1:b]
# drop comment: start at the line after the comment end
i0 = next(i for i, l in enumerate(lines) if l.strip().endswith("*/"))
head = "/* %s */" % out[0].split()[0].split("=")[1]
body = lines[i0+1:]
sw = next(i for i, l in enumerate(body) if "switch (" in l)
sites = [i for i, l in enumerate(body) if i > sw and l.strip() == "return;"]
print("# %d sites: %s" % (len(sites), [s + a + i0 + 1 for s in sites]), file=sys.stderr)
def emit(label, chosen):
    t = list(body)
    for s in chosen:
        t[s] = t[s].replace("return;", "break;")
    print("### " + label); print(head); print("\n".join(t)); print()
if mode == "single":
    emit("all", sites)
    for k, s in enumerate(sites):
        emit("s%d" % k, [s])
    for k, s in enumerate(sites):
        emit("allbut%d" % k, [x for x in sites if x != s])
elif mode == "mask":
    for m in sys.argv[3].split(","):
        idx = [int(x) for x in m.split("+")] if m else []
        emit("m" + m, [sites[i] for i in idx])
