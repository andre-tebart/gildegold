"""initgen.py SRC FUNC OUT [BASEFILE]: one dcp try variant per local variable, declared with `= 0`.
Locals are read from every declaration run of the body: the run at the top of the function (blank lines and
comments inside it included), a later run at function scope, and the declarations that open each inner block
(C89: a block starts with its declarations). A declaration line the function has more than once is picked with
`<<<N`. BASEFILE: an optional variants-file body (blocks) prepended to each variant."""
import re, sys

src, func, out = sys.argv[1], sys.argv[2], sys.argv[3]
base = open(sys.argv[4]).read() if len(sys.argv) > 4 else ""
t = open(src, encoding="latin-1").read()
# a definition: its line starts in column 0 (a call inside another function's body is indented); a K&R
# definition has its parameters' declarations (`int x;`) between the `)` and the `{`
m = re.search(r"(?m)^(?![ \t])[^\n;{}=]*\b" + re.escape(func) + r"\([^;{]*\)\s*(?:/\*.*?\*/\s*)?"
              r"(?:[A-Za-z_][^\n;{}()=]*;[ \t]*\n)*\{\n", t)
if not m:
    sys.exit("initgen: no definition of %s in %s" % (func, src))
body = t[m.end():]
KEYWORDS = {"return", "if", "else", "for", "while", "do", "switch", "case", "default", "goto", "break", "continue",
            "sizeof"}


def is_decl(s):
    """a declaration statement: `TYPE name[, *name ..];` (an assignment `x = 1;`, `i += 2;` or a call is none)"""
    if not s.endswith(";"):
        return False
    if "(" in s.split("=")[0] and not re.match(r"^[A-Za-z_][\w\s\*]*\(\*", s):
        return False
    m3 = re.match(r"^(?:(?:const|volatile|unsigned|signed|struct|union|enum|static|register)\s+)*"
                  r"([A-Za-z_]\w*)\s+([*A-Za-z_(])", s)
    return bool(m3) and m3.group(1) not in KEYWORDS


lines = []      # (line, its index among the body's lines)
blines = body.split("\n")
depth = 0       # brace depth inside the body (0 = function scope)
skip = 0        # inside a local struct / union type's member list (`union { .. } p;`): skipped
opened = True   # at the start of a block: declarations may follow
nbody = len(blines)
for k, ln in enumerate(blines):
    s = ln.strip()
    if skip:
        skip += s.count("{") - s.count("}")
        continue
    if re.match(r"^(?:struct|union)\b[^;]*\{\s*$", s):
        skip = 1
        continue
    if not s or s.startswith("/*") or s.startswith("*") or s.startswith("//"):
        continue
    if is_decl(s):
        if opened or depth == 0:
            lines.append((ln, k))
        continue
    opened = False
    code = re.sub(r'"(?:\\.|[^"\\])*"' + r"|'(?:\\.|[^'\\])*'|/\*.*?\*/", "", s)
    for ch in code:
        if ch == "{":
            depth += 1
            opened = True
        elif ch == "}":
            depth -= 1
            opened = False
    if depth < 0:
        nbody = k + 1   # the function's closing brace
        break
fn = "\n".join(blines[:nbody])
INT_WORDS = {"int", "char", "short", "long", "unsigned", "signed", "BOOL", "BYTE", "WORD", "DWORD", "UINT",
             "ULONG", "LONG", "HANDLE", "HWND", "size_t", "uint8_t", "uint16_t", "uint32_t", "int8_t", "int16_t",
             "int32_t", "u8", "u16", "u32", "s8", "s16", "s32"}


def scalar_type(typ):
    """True for an integer type (and an unknown lower-case typedef); False for float / double and for an
    upper-case typedef or struct, which is a record unless it is a known integer name."""
    words = [w for w in typ.split() if w not in ("const", "volatile", "struct")]
    last = words[-1] if words else typ
    if last in ("float", "double") or "struct" in typ.split():
        return False
    if last in INT_WORDS:
        return True
    return not re.match(r"^[A-Z0-9_]+$", last)


def marker(ln, k):
    """`<<<` for a line the function has once, else `<<<N` (N: its occurrence, as dcp try counts them)"""
    if fn.count(ln) == 1:
        return "<<<"
    before = "\n".join(blines[:k])
    return "<<<%d" % (before.count(ln) + 1)


res = []
names = {}
for ln, k in lines:
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
        if not p.startswith("*") and not scalar_type(typ):
            continue  # a float `= 0` emits a store; a struct `= 0` does not compile
        np = list(parts)
        np[i] = p + " = 0"
        newln = ind + typ + " " + ", ".join(np) + ";"
        if m2.group(4):
            newln += " " + m2.group(4)
        names[name] = names.get(name, 0) + 1
        label = name if names[name] == 1 else "%s-%d" % (name, names[name])   # one name in two blocks
        res.append((label, marker(ln, k), ln, newln))
with open(out, "w", newline="\n") as f:
    for name, mk, old, new in res:
        f.write("### init-%s\n%s%s\n%s\n===\n%s\n>>>\n\n" % (name, base, mk, old, new))
print(len(res), "variants:", " ".join(r[0] for r in res))
