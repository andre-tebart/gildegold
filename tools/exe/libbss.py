"""libbss: LIBC's .bss member by member, ours (from a /VERBOSE relink) against the original's.

    python tools/exe/libbss.py                # our LIBC .bss layout, the original's runs, the members only ours has
    python tools/exe/libbss.py --pulls        # also every LIBC member our link loads: symbol and who referenced it
    python tools/exe/libbss.py --order a.obj,b.obj,..   # simulate another member order from the same start

Why (data layout s13): LINK puts the units' .bss in link order, then LIBC's members' .bss in their LOAD order
(each member's .bss section aligned on its own: time / cfin / cfout / tzset / gmtime 8, 87cdisp 16), then the
common area (32-aligned for a big first common). The map lists only public symbols, so static .bss of a member
(strerror's errmsg, time's caches, asctime's buf) is invisible there; this tool reads every member's .bss size and
alignment from LIBC.LIB itself and lays them out in our load order. The load order comes from `link /VERBOSE`
("Found SYM / Referenced in OBJ / Loaded LIBC.LIB(member)"): it reruns build/link/exe/link.rsp into a scratch
directory (the real build is not touched; set DCP_TOOLCHAIN to the MSVC root if it is not C:\\Tools\\MSVC600-SP5PP).
The original's side is `dcp answers` over the same range (unit=lib:LIBC.LIB/x.obj); its open runs are members
without a public symbol there (_file's _cflush, _newmode, nlsdata2 + ncommode, txtmode) or alignment pads.
Found with it (s13): our only extra LIBC member is strerror.obj (40-byte .bss, pulled by gzio's zstrerror); the
original pulled syserr.obj through a member with no .bss and no surviving text (perror.obj fits: a removed
function's perror call), and the rest of the +48 is alignment from the member order (crt0dat before time, fpinit
before fprintf -> cvt before _sftbuf, atof early -> cfin after 87cdisp): all pulled by removed functions.
"""
import argparse, os, re, struct, subprocess, sys, tempfile

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
MSVC = os.environ.get("DCP_TOOLCHAIN", r"C:\Tools\MSVC600-SP5PP")
LIB = os.path.join(MSVC, "VC98", "Lib", "LIBC.LIB")
COMMON_START = 0x6f3300  # the original's first common; its LIBC .bss ends exactly there


def members(path):
    b = open(path, "rb").read()
    off, longnames = 8, b""
    while off < len(b):
        hdr = b[off:off + 60]
        name = hdr[:16].decode("latin-1").strip()
        size = int(hdr[48:58].decode().strip())
        data = b[off + 60:off + 60 + size]
        if name == "//":
            longnames = data
        elif name.startswith("/") and name[1:].strip().isdigit():
            i = int(name[1:])
            yield longnames[i:longnames.index(b"\0", i)].decode("latin-1"), data
        elif name not in ("/", ""):
            yield name.rstrip("/"), data
        off += 60 + size + (size & 1)  # members are 2-aligned


def bss_sections(data):
    """[(size, align, first symbol)] of the member's .bss sections."""
    mach, nsec, _, symoff, nsym, opt, _ = struct.unpack_from("<HHIIIHH", data, 0)
    if mach != 0x14c:
        return []
    secs = []
    for i in range(nsec):
        o = 20 + opt + i * 40
        nm = data[o:o + 8].rstrip(b"\0").decode("latin-1")
        rawsz = struct.unpack_from("<I", data, o + 16)[0]
        ch = struct.unpack_from("<I", data, o + 36)[0]
        al = (ch >> 20) & 0xf
        secs.append([nm, rawsz, (1 << (al - 1)) if al else 16, None])
    stro = symoff + nsym * 18
    i = 0
    while i < nsym:
        o = symoff + i * 18
        if data[o:o + 4] == b"\0\0\0\0":
            so = struct.unpack_from("<I", data, o + 4)[0]
            nm = data[stro + so:data.index(b"\0", stro + so)].decode("latin-1")
        else:
            nm = data[o:o + 8].rstrip(b"\0").decode("latin-1")
        val, sec, _, cls, naux = struct.unpack_from("<IhHBB", data, o + 8)
        if sec > 0 and cls in (2, 3) and naux == 0 and val == 0 and secs[sec - 1][3] is None:
            secs[sec - 1][3] = nm
        i += 1 + naux
    return [(s[1], s[2], s[3] or "") for s in secs if s[0] == ".bss" and s[1]]


def lib_bss():
    out = {}
    for nm, data in members(LIB):
        base = nm.replace("\\", "/").split("/")[-1].lower()
        if base.endswith(".obj") and base not in out:
            try:
                out[base] = bss_sections(data)
            except struct.error:
                out[base] = []
    return out


def verbose_link():
    """[(member, symbol, referenced in)] in load order, from a /VERBOSE relink into a scratch directory."""
    tmp = tempfile.mkdtemp(prefix="libbss_")
    rsp = open(os.path.join(ROOT, "build", "link", "exe", "link.rsp")).read()
    rsp = re.sub(r"^/OUT:.*$", "/OUT:" + os.path.join(tmp, "v.exe").replace("\\", "\\\\"), rsp, flags=re.M)
    rsp = re.sub(r"^/MAP:.*$", lambda m: "/MAP:" + os.path.join(tmp, "v.map") + "\n/VERBOSE", rsp, flags=re.M)
    p = os.path.join(tmp, "v.rsp")
    open(p, "w").write(rsp)
    env = dict(os.environ)
    env["PATH"] = os.pathsep.join([os.path.join(MSVC, "VC98", "Bin"), os.path.join(MSVC, "Common", "MSDev98", "Bin"),
                                   env.get("PATH", "")])
    env["LIB"] = os.path.join(MSVC, "VC98", "Lib")
    r = subprocess.run([os.path.join(MSVC, "VC98", "Bin", "LINK.EXE"), "@" + p], cwd=ROOT, env=env,
                       capture_output=True, text=True, encoding="latin-1")
    if r.returncode:
        sys.exit("libbss: the /VERBOSE relink failed:\n" + (r.stdout + r.stderr)[-1500:])
    pulls, sym, ref = [], None, None
    for line in r.stdout.splitlines():
        s = line.strip()
        if s.startswith("Found "):
            sym, ref = s[6:], None
        elif s.startswith("Referenced in ") and ref is None:
            ref = s[14:]
        else:
            m = re.match(r"Loaded LIBC\.LIB\((.+)\)", s)
            if m:
                pulls.append((m.group(1).lower(), sym, ref))
    return pulls


def obj_units():
    first = {}
    for l in open(os.path.join(ROOT, "build", "link", "exe", "exe.map")):
        p = l.split()
        if len(p) >= 4 and p[0].startswith("0001:") and p[-1].endswith(".obj") and ":" not in p[-1]:
            first.setdefault(p[-1], p[1])
    return first


def layout(order, bss, start):
    rows, a = [], start
    for m in order:
        for size, al, sym in bss.get(m, []):
            pad = (-a) % al
            a += pad
            rows.append((a, size, al, pad, m, sym))
            a += size
    return rows, a


def original_runs(lo, hi):
    r = subprocess.run(["dcp", "answers", "%#x..%#x" % (lo, hi)], cwd=ROOT, capture_output=True, text=True,
                       encoding="utf-8", errors="replace")
    runs = []
    for l in r.stdout.splitlines():
        m = re.match(r"(0x[0-9a-f]+)(?:\.\.(0x[0-9a-f]+))?\s+(\d+)\s+(\S+)\s+\S+\s*(\S*)(?:.*unit=lib:LIBC\.LIB/(\S+))?", l)
        if m:
            runs.append((int(m.group(1), 16), int(m.group(3)), m.group(4), m.group(5), (m.group(6) or "").lower()))
    return runs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--pulls", action="store_true", help="print every LIBC member load with its trigger")
    ap.add_argument("--order", help="comma-separated member order to simulate instead of our load order")
    ap.add_argument("--start", type=lambda s: int(s, 0), help="LIBC .bss start (default: our first LIBC .bss)")
    a = ap.parse_args()
    bss = lib_bss()
    pulls = verbose_link()
    units = obj_units()
    if a.pulls:
        for m, sym, ref in pulls:
            ref = ref or ""
            print("  %-16s %-36s <- %s" % (m, sym, units.get(os.path.basename(ref), ref)))
    order = [m for m, _, _ in pulls]
    # our LIBC .bss start: the first member with a .bss, at the map address of its public symbol
    start = a.start
    if start is None:
        pub = {}
        for l in open(os.path.join(ROOT, "build", "link", "exe", "exe.map")):
            p = l.split()
            if len(p) >= 4 and p[0].startswith("0003:") and p[-1].startswith("LIBC:"):
                pub[p[1]] = int(p[2], 16)
        for m in order:
            if bss.get(m):
                first = bss[m][0][2]
                if first in pub:
                    start = pub[first]
                    break
                sys.exit("libbss: the first LIBC .bss member %s has no public symbol; pass --start" % m)
    rows, end = layout(order, bss, start)
    print("ours (load order):")
    for addr, size, al, pad, m, sym in rows:
        print("  %#010x %4d al%-2d pad %2d  %-14s %s" % (addr, size, al, pad, m, sym))
    print("  end %#x (original %#x: %+d), common area from %#x" % (end, COMMON_START, end - COMMON_START,
                                                                  (end + 31) & ~31))
    if a.order:
        rows2, end2 = layout([m.strip().lower() for m in a.order.split(",")], bss, start)
        print("simulated order:")
        for addr, size, al, pad, m, sym in rows2:
            print("  %#010x %4d al%-2d pad %2d  %-14s %s" % (addr, size, al, pad, m, sym))
        print("  end %#x (%+d)" % (end2, end2 - COMMON_START))
    runs = original_runs(start, COMMON_START)
    print("original (dcp answers):")
    named = set()
    for addr, n, kind, sym, mem in runs:
        if kind == "global":
            continue
        named.add(mem)
        print("  %#010x %4d %-12s %-14s %s" % (addr, n, kind, mem, sym))
    extra = [(m, sum(s for s, _, _ in bss[m])) for m in dict.fromkeys(order) if bss.get(m) and m not in named]
    print("our members with .bss the original's answers do not name (an open run must hold them, or the original"
          " did not link them):")
    for m, s in extra:
        print("  %-14s %4d" % (m, s))


if __name__ == "__main__":
    main()
