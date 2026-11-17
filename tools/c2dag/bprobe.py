"""boundary probe: where can a unit split, judged by the code (callee summaries), not the data.
bprobe.py TREE UNIT P1 [P2 ..]   TREE: a source tree (the repo root, or `git archive COMMIT | tar -x -C DIR` for a
unit as it was before a split)   (Pi = hex address of the first function of the emulated second unit)
For each Pi: before Pi's definition, every function defined above becomes an external (`#define f f_x` + extern
prototype), so every function from Pi on sees none of the earlier ones (as after a split; data definitions stay).
Lists functions whose code changes vs the merged compile, with distance to the original merged -> split.
'ALL' as P compiles nothing extra (base only), printing the base distances of functions in a range with --range."""
import os, re, sys, pickle, subprocess
sys.path.insert(0, r"C:\Users\andre\Documents\gildegold\tools\c2dag")
import unitprobe as up

tree, unit = os.path.abspath(sys.argv[1]), sys.argv[2]
LST = [a for a in sys.argv[3:] if a.startswith("list:")]
Ps = [int(x, 16) for x in sys.argv[3:] if not x.startswith("list:")]
src = open(os.path.join(tree, "src", unit + ".c"), encoding="latin-1", newline="").read().replace("\r\n", "\n")
L = src.split("\n")

# function definitions: header lines up to a line "{" ; address from the nearest preceding "/* 0x" comment
defs = []  # (start_line, name, protolines, addr)
i = 0
while i < len(L):
    l = L[i]
    m = re.match(r"^(?:static |__inline |extern )?[A-Za-z_][\w \*]*?\b(\w+)\(", l)
    if m and not l.rstrip().endswith(";") and not l.startswith("#") and not l.startswith("typedef"):
        j = i
        while j < len(L) and L[j] != "{" and not L[j].rstrip().endswith(";") and j - i < 8:
            j += 1
        if j < len(L) and L[j] == "{":
            k = i - 1
            addr = None
            while k >= 0 and i - k < 40:
                mm = re.match(r"^/\* (0x[0-9a-f]+)", L[k])
                if mm:
                    addr = int(mm.group(1), 16)
                    break
                if L[k] == "}":
                    break
                k -= 1
            defs.append((i, m.group(1), L[i:j], addr, k if addr else i))
            i = j
    i += 1

ROOT = up.ROOT


def compile_t(text, label):
    W = up.W
    s = os.path.join(W, label + ".c")
    open(s, "w", encoding="latin-1", newline="").write(text.replace("\n", "\r\n"))
    asm, obj = os.path.join(W, label + ".asm"), os.path.join(W, label + ".obj")
    cmd = [os.path.join(up.BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/FAc", "/I" + os.path.join(tree, "include"),
           "/I" + os.path.join(tree, "src"), r"/IC:\Tools\DXSDK61\include", "/Fa" + asm, "/Fo" + obj, s]
    r = subprocess.run(cmd, env=up.ENV, cwd=tree, capture_output=True, text=True, timeout=600)
    if r.returncode:
        raise SystemExit("compile failed: " + "\n".join(x for x in r.stdout.splitlines() if "error" in x)[:2000])
    # reuse unitprobe's parser by faking: call its internals
    offs, cur, name = {}, None, None
    for l in open(asm, encoding="latin-1"):
        m = re.match(r"([_@?]\S+)\s+PROC", l)
        if m:
            name, cur = m.group(1).lstrip("_"), []
            continue
        if re.match(r"[_@?]\S+\s+ENDP", l):
            offs[name] = cur
            cur = None
            continue
        if cur is not None:
            m = re.match(r"  ([0-9a-f]{5})\t", l)
            if m:
                cur.append(int(m.group(1), 16))
    fns = {}
    for nm, (data, rel) in up.coff(obj).items():
        if nm not in offs:
            continue
        data = data.rstrip(bytes([0x90]))
        o = offs[nm] + [len(data)]
        ins = []
        for a, b in zip(o, o[1:]):
            if b <= a:
                continue
            x = up.mask_jump(data[a:b])
            x = tuple(None if (a + k) in rel else v for k, v in enumerate(x))
            if any(v is not None for v in x):
                ins.append(x)
        fns[nm] = ins
    os.remove(obj)
    return fns


CACHE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "orig_cache.pkl")
oc = pickle.load(open(CACHE, "rb")) if os.path.exists(CACHE) else {}


def orig(addr):
    if addr not in oc:
        oc[addr] = up.original("0x%x" % addr)
        pickle.dump(oc, open(CACHE, "wb"))
    return oc[addr]


addr_of = {d[1]: d[3] for d in defs}
for a in LST:
    lo, hi = [int(x, 16) for x in a[5:].split("-")]
    for n, d in enumerate(defs):
        if d[3] and lo <= d[3] <= hi:
            print("  %d 0x%x %s" % (n, d[3], d[1]))
if LST and not Ps:
    raise SystemExit(0)
tag = os.path.basename(tree.rstrip("/\\")) + "_" + unit
bp = os.path.join(up.W, "bp_base_%s.pkl" % tag)
if os.path.exists(bp):
    base = pickle.load(open(bp, "rb"))
else:
    base = compile_t(src, "bp_base_" + tag)
    pickle.dump(base, open(bp, "wb"))

for P in Ps:
    idx = [n for n, d in enumerate(defs) if d[3] == P]
    if not idx:
        print("no function at 0x%x" % P)
        continue
    n0 = idx[0]
    pre = []
    for d in defs[:n0]:
        proto = " ".join(x.strip() for x in d[2])
        proto = re.sub(r"^(static|__inline)\s+", "", proto)
        proto = re.sub(r"\b%s\(" % d[1], d[1] + "_x(", proto, count=1)
        pre.append("extern %s;\n#define %s %s_x" % (proto, d[1], d[1]))
    ins_line = defs[n0][4]
    t = "\n".join(L[:ins_line] + pre + L[ins_line:])
    new = compile_t(t, "bp_%s_%x" % (tag, P))
    names_after = [d[1] for d in defs[n0:]]
    ch = [k for k in names_after if k in base and k in new and base[k] != new[k]]
    rows = []
    for k in ch:
        o = orig(addr_of[k]) if addr_of.get(k) else None
        rows.append("%s(0x%x) %d->%d" % (k, addr_of.get(k) or 0, up.dist(base[k], o), up.dist(new[k], o)) if o else k + " (no orig)")
    print("split at 0x%x (%s): changed %d: %s" % (P, defs[n0][1], len(ch), "; ".join(rows)))
