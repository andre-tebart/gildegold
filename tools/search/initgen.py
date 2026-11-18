"""initgen.py SRC FUNC OUT [BASEFILE]: one dcp try variant per local variable, declared with `= 0`.
Locals are read from the declaration lines at the top of the function body (before the first blank line
or statement). BASEFILE: an optional variants-file body (blocks) prepended to each variant."""
import re, sys

src, func, out = sys.argv[1], sys.argv[2], sys.argv[3]
base = open(sys.argv[4]).read() if len(sys.argv) > 4 else ""
t = open(src, encoding="latin-1").read()
m = re.search(r"(?m)^[^\n;{}]*\b" + re.escape(func) + r"\([^;{]*\)\s*\{\n", t)
body = t[m.end():]
lines = []
for ln in body.split("\n"):
    s = ln.strip()
    if not s or s.startswith("/*") or s.startswith("*") or s.startswith("//"):
        if lines and not s:
            break
        continue
    if not s.endswith(";") or "(" in s.split("=")[0] and not re.match(r"^[A-Za-z_][\w\s\*]*\(\*", s):
        break
    if re.match(r"^(return|if|for|while|do|switch)\b", s):
        break
    lines.append(ln)
res = []
for ln in lines:
    m2 = re.match(r"^(\s*)((?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+)*[A-Za-z_]\w*)\s+(.*);\s*(/\*.*\*/)?\s*$", ln)
    if not m2:
        continue
    ind, typ, decls = m2.group(1), m2.group(2), m2.group(3)
    parts = [p.strip() for p in decls.split(",")]
    for i, p in enumerate(parts):
        if "=" in p or "[" in p:
            continue
        name = p.lstrip("*").strip()
        if not re.match(r"^\w+$", name):
            continue
        np = list(parts)
        np[i] = p + " = 0"
        newln = ind + typ + " " + ", ".join(np) + ";"
        if m2.group(4):
            newln += " " + m2.group(4)
        res.append((name, ln, newln))
with open(out, "w", newline="\n") as f:
    for name, old, new in res:
        f.write("### init-%s\n%s<<<\n%s\n===\n%s\n>>>\n\n" % (name, base, old, new))
print(len(res), "variants:", " ".join(r[0] for r in res))
