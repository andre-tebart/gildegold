"""Whole-unit probes: what a changed callee does to its callers (dcp try grades one function only).

  unitprobe.py UNIT PATCHFILE [LABEL..] [/flag..]   scratch copies of src/UNIT.c with the patches applied, compiled
      /O2 /Gy; every function's code (obj bytes, relocations and jump displacements masked) compared with the
      unpatched compile, each changed function graded against the original (dcp disasm): differing instructions
      base -> variant (a scratch measure: jump tables of the original count as noise, compare the two numbers only).
      PATCHFILE: `### label` sections of blocks `<<<` old `===` new `>>>` (each old text once in the unit).
  unitprobe.py UNIT --unknown FUNC                  one variant per same-unit callee of FUNC defined above it: FUNC
      calls a renamed external prototype instead (what VC6 knows of that callee is gone), and one with all.
  unitprobe.py UNIT base                            refresh the unpatched compile after the unit changed.
Nothing in the tree is written; the work files go to build/c2dag/unitprobe (ignored)."""
import os, re, subprocess, sys, pickle, struct, difflib

HERE = os.path.dirname(os.path.abspath(__file__))
VC = r"C:\Tools\MSVC600-SP5PP"
BIN = VC + r"\VC98\Bin"
ROOT = r"C:\Users\andre\Documents\gildegold"
ENV = dict(os.environ)
ENV["INCLUDE"] = VC + r"\VC98\Include"
ENV["PATH"] = VC + r"\Common\MSDev98\Bin;" + ENV["PATH"]
W = os.path.normpath(os.path.join(HERE, "..", "..", "build", "c2dag", "unitprobe"))
os.makedirs(W, exist_ok=True)


def coff(path):
    """name -> (data, set of reloc offsets) per function symbol"""
    d = open(path, "rb").read()
    mach, nsec, _, symoff, nsym, opt, _ = struct.unpack_from("<HHIIIHH", d, 0)
    secs = []
    for i in range(nsec):
        o = 20 + opt + 40 * i
        name, vs, va, rawsz, rawptr, relptr, _, nrel, _, ch = struct.unpack_from("<8sIIIIIIHHI", d, o)
        secs.append((rawsz, rawptr, relptr, nrel))
    strtab = symoff + 18 * nsym
    out = {}
    i = 0
    while i < nsym:
        o = symoff + 18 * i
        raw, val, sec, typ, cls, naux = struct.unpack_from("<8sIhHBB", d, o)
        if raw[:4] == bytes(4):
            so = struct.unpack_from("<I", raw, 4)[0]
            name = d[strtab + so:d.index(bytes(1), strtab + so)].decode("latin-1")
        else:
            name = raw.rstrip(bytes(1)).decode("latin-1")
        if sec > 0 and typ == 0x20 and cls == 2 and val == 0:
            rawsz, rawptr, relptr, nrel = secs[sec - 1]
            rel = set()
            for k in range(nrel):
                ra = struct.unpack_from("<I", d, relptr + 10 * k)[0]
                rel.update(range(ra, ra + 4))
            out[name.lstrip("_")] = (d[rawptr:rawptr + rawsz], rel)
        i += 1 + naux
    return out


def mask_jump(b):
    """instruction bytes -> tuple with jump / call displacements as None"""
    b = list(b)
    if not b:
        return tuple(b)
    if b[0] in (0xe8, 0xe9) and len(b) == 5:
        b[1:] = [None] * 4
    elif (b[0] == 0xeb or 0x70 <= b[0] <= 0x7f or b[0] in (0xe3, 0xe2)) and len(b) == 2:
        b[1] = None
    elif b[0] == 0x0f and len(b) == 6 and 0x80 <= b[1] <= 0x8f:
        b[2:] = [None] * 4
    return tuple(b)


def compile_(text, label, flags=()):
    src = os.path.join(W, label + ".c")
    open(src, "w", encoding="latin-1", newline="").write(text)
    asm = os.path.join(W, label + ".asm")
    obj = os.path.join(W, label + ".obj")
    cmd = [os.path.join(BIN, "CL.EXE"), "/nologo", "/c", "/O2", "/Gy", "/FAc"] + list(flags) + \
          ["/I" + ROOT + r"\include", "/I" + ROOT + r"\src", r"/IC:\Tools\DXSDK61\include", "/Fa" + asm,
           "/Fo" + obj, src]
    r = subprocess.run(cmd, env=ENV, cwd=ROOT, capture_output=True, text=True, timeout=300)
    if r.returncode:
        raise SystemExit("compile failed: " + "\n".join(l for l in r.stdout.splitlines() if "error" in l)[:1500])
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
    for name, (data, rel) in coff(obj).items():
        if name not in offs:
            continue
        data = data.rstrip(bytes([0x90]))
        o = offs[name] + [len(data)]
        ins = []
        for a, b in zip(o, o[1:]):
            if b <= a:
                continue
            x = mask_jump(data[a:b])
            x = tuple(None if (a + k) in rel else v for k, v in enumerate(x))
            if any(v is not None for v in x):
                ins.append(x)
        fns[name] = ins
    os.remove(obj)
    return fns


def original(name):
    r = subprocess.run(["dcp", "disasm", name], cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    ins = []
    for l in r.stdout.splitlines():
        m = re.match(r"\s+0x[0-9a-f]{8}\s+([0-9a-f]+)\s", l)
        if m and "; fill" not in l:
            ins.append(mask_jump(bytes.fromhex(m.group(1))))
    return ins


def dist(ours, orig):
    """instructions outside equal runs (ours' masked bytes are wildcards); paired on a coarse key, then verified"""
    def eq(a, b):
        return len(a) == len(b) and all(x is None or y is None or x == y for x, y in zip(a, b))
    ck = lambda x: (len(x), x[0], x[1] if len(x) > 1 and len(x) < 5 else -1)
    sm = difflib.SequenceMatcher(None, [ck(x) for x in ours], [ck(x) for x in orig], autojunk=False)
    same = 0
    for a, b, n in sm.get_matching_blocks():
        same += sum(1 for k in range(n) if eq(ours[a + k], orig[b + k]))
    return max(len(ours), len(orig)) - same


def unknown_variants(text, fn):
    """the patch text of one variant per same-unit callee of fn defined above it"""
    L = text.split("\n")
    defs = {}
    for i, l in enumerate(L):
        m = re.match(r"^(?:static )?[A-Za-z_][\w \*]*?\b(\w+)\(([^;{]*)\)\s*$", l)
        if m and i + 1 < len(L) and L[i + 1].startswith("{"):
            defs.setdefault(m.group(1), (i, l))
    i0 = defs[fn][0]
    end = next(k for k in range(i0, len(L)) if L[k] == "}")
    body = "\n".join(L[i0:end + 1])
    anchor = "\n".join(L[i0:i0 + 3])
    assert text.count(anchor) == 1
    callees = [c for c in dict.fromkeys(re.findall(r"\b(\w+)\(", body)) if c in defs and defs[c][0] < i0 and c != fn]
    out, alls = [], []
    for c in callees:
        pre = "extern %s;\n#define %s %s_x\n" % (re.sub(r"\b%s\(" % c, c + "_x(", defs[c][1]), c, c)
        alls.append(pre)
        out.append("### %s\n<<<\n%s\n===\n%s%s\n>>>\n" % (c, anchor, pre, anchor))
    out.append("### ALL\n<<<\n%s\n===\n%s%s\n>>>\n" % (anchor, "".join(alls), anchor))
    return "".join(out)


if __name__ == "__main__":
    if len(sys.argv) < 3 or sys.argv[1] in ("-h", "--help"):
        print(__doc__ or "usage: unitprobe.py UNIT (base | FILE | --unknown FUNC ...)")
        sys.exit(0 if len(sys.argv) > 1 else 2)
    UNIT = sys.argv[1]
    args = sys.argv[2:]
    if not os.path.exists(os.path.join(ROOT, "src", UNIT + ".c")):
        sys.exit("unitprobe: no unit src/%s.c" % UNIT)
    base_text = open(os.path.join(ROOT, "src", UNIT + ".c"), encoding="latin-1", newline="").read()
    bp = os.path.join(W, "base_%s.pkl" % UNIT)
    if args[0] == "base" or not os.path.exists(bp):
        base = compile_(base_text, "base_" + UNIT)
        pickle.dump(base, open(bp, "wb"))
        print("base: %d functions" % len(base))
        if args[0] == "base":
            raise SystemExit(0)
    base = pickle.load(open(bp, "rb"))
    crlf = "\r\n" in base_text
    if args[0] == "--unknown":
        full = unknown_variants(base_text.replace("\r\n", "\n"), args[1])
        args = args[1:]
    else:
        full = open(args[0], encoding="latin-1", newline="").read().replace("\r\n", "\n")
    secs = re.split(r"^### *(\S+).*\n", full, flags=re.M)
    secs = [("v", full)] if len(secs) == 1 else list(zip(secs[1::2], secs[2::2]))
    want = [a for a in args[1:] if not a.startswith("/")]
    flags = [a for a in args[1:] if a.startswith("/")]
    for lab, body in secs:
        if want and lab not in want:
            continue
        t = base_text.replace("\r\n", "\n")
        bad = False
        for m in re.finditer(r"<<<\n(.*?)\n===\n(.*?)\n?>>>", body, re.S):
            old, new_ = m.group(1), m.group(2)
            if t.count(old) != 1:
                print("%s: patch old text found %d times: %r" % (lab, t.count(old), old[:80]))
                bad = True
                break
            t = t.replace(old, new_)
        if bad:
            continue
        if crlf:
            t = t.replace("\n", "\r\n")
        try:
            new = compile_(t, lab, flags)
        except SystemExit as e:
            print(lab, ":", e)
            continue
        ch = [k for k in base if k in new and base[k] != new[k]]
        gone = [k for k in base if k not in new]
        add = [k for k in new if k not in base]
        rows = []
        for k in ch:
            o = original(k)
            rows.append("%s %d->%d" % (k, dist(base[k], o), dist(new[k], o)) if o else k + " (no original)")
        print("%s: changed %d: %s%s%s" % (lab, len(ch), ", ".join(rows[:60]),
              " gone: " + " ".join(gone[:10]) if gone else "", " new: " + " ".join(add[:10]) if add else ""))
