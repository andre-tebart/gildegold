"""returnbreak.py (deep s24): the return-as-break round for `dcp try`.

    python tools/sweep/returnbreak.py src/he_b.c he_RunSecretRoom 0x56e5b0 v.txt && dcp try he_RunSecretRoom v.txt

Writes one whole-function variant per `return;` inside the function's first top-level switch, that one return
written as `break;`. Only meaningful when the switch is the function's last statement and the return is not
inside a loop: check a byte hit's line before applying it.
"""
import re, sys
path, func, addr, outp = sys.argv[1:5]
src = open(path, encoding="latin-1").read()
m = re.search(r"\n[^\n;{}]*\b%s\([^)]*\)\n\{" % re.escape(func), src)
i = m.start() + 1
j = src.find("\n}\n", i) + 3
fn = src[i:j]
lines = fn.split("\n")
sw = [k for k, l in enumerate(lines) if re.match(r"^    switch \(", l)]
if not sw:
    sys.stderr.write("no switch\n"); sys.exit(1)
sw = sw[0]
# end of that switch: the line "    }" after sw
end = next(k for k in range(sw + 1, len(lines)) if lines[k] == "    }")
cands = [k for k in range(sw, end) if lines[k].strip() == "return;"]
out = []
for n, k in enumerate(cands):
    ls = list(lines)
    ls[k] = ls[k].replace("return;", "break;")
    out.append("### b%d_L%d\n/* %s */\n%s\n" % (n, k, addr, "\n".join(ls)))
open(outp, "w", encoding="latin-1").write("".join(out))
sys.stderr.write("%s: %d candidates\n" % (func, len(cands)))
