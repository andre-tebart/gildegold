"""datameasure: the linked exe's data (.rdata, .data, .bss) against the original, symbol by symbol.

    python tools/exe/datameasure.py              # summary and the first steps
    python tools/exe/datameasure.py --steps -n 80 [--section rdata|data|bss] [--unit U]
    python tools/exe/datameasure.py --refresh    # re-read `dcp data map` of every unit (cached in build/exe/datamap)
    python tools/exe/datameasure.py --sizes [-n N]  # initialised .data per unit, ours vs the original, and the
                                                 # symbols one side keeps initialised and the other zero (.bss)

Why: function placement is only half of the exe. A data symbol sits at its original address only when everything
before it in its section has the original's size, order and owner unit. A STEP is a symbol where the shift
(our address - original address) changes: the cause sits between it and the step before. The usual causes:
- size < room: the original has more after this datum (an undeclared neighbour, a string or float literal of the
  unit, alignment of a char / short, a bigger array);
- the datum is defined in another unit than the original's neighbours (it is placed with its own unit's data);
- the datum is in another section (const / initialised / zero: VC6 puts `= 0` into .data, not .bss;
  string literals go to .data, `const` tables to .rdata);
- definition order inside a unit (VC6 emits a unit's data in definition order per section).
Reads build/link/exe/exe.map (run `dcp link` or tools/exe/exemeasure.py first) and `dcp data map UNIT`.
"""
import argparse, glob, os, re, struct, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
CACHE = os.path.join(ROOT, "build", "exe", "datamap")
MAP = os.path.join(ROOT, "build", "link", "exe", "exe.map")
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import exemeasure  # noqa: E402


def refresh():
    os.makedirs(CACHE, exist_ok=True)
    r = subprocess.run(["dcp", "data", "map"], cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    units = re.findall(r"unit=(\S+)", r.stdout)
    for u in units:
        r = subprocess.run(["dcp", "data", "map", u], cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
        open(os.path.join(CACHE, u + ".txt"), "w", encoding="utf-8").write(r.stdout)
    return len(units)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--steps", action="store_true")
    ap.add_argument("--section")
    ap.add_argument("--unit")
    ap.add_argument("-n", type=int, default=25)
    ap.add_argument("--refresh", action="store_true")
    ap.add_argument("--sizes", action="store_true")
    a = ap.parse_args()
    if a.refresh or not glob.glob(os.path.join(CACHE, "*.txt")):
        print("data map: %d units read" % refresh())
    decl = {}
    for f in glob.glob(os.path.join(CACHE, "*.txt")):
        u = os.path.basename(f)[:-4]
        for l in open(f, encoding="utf-8", errors="replace"):
            m = re.search(r"at=0x([0-9a-f]+)\s+name=(\S+).*?section=(\S+)\s+size=(\d+)\s+room=(\S+).*?src=(\S+)", l)
            if m:
                decl[m.group(2)] = (int(m.group(1), 16), m.group(3), int(m.group(4)), m.group(5), u, m.group(6))
    ours = {}
    bss_start = None
    dsyms = []  # (our address, obj) of every .data/.bss symbol of the map
    for l in open(MAP, encoding="latin-1"):
        m = re.match(r"\s*(000[23]):([0-9a-f]+)\s+_?(\S+)\s+([0-9a-f]{8})\s+(\S+)", l)
        if m:
            ours.setdefault(m.group(3), int(m.group(4), 16))
            if m.group(1) == "0003":
                dsyms.append((int(m.group(4), 16), m.group(5), m.group(3)))
        m = re.match(r"\s*0003:([0-9a-f]+)\s+[0-9a-f]+H\s+\.bss\s", l)
        if m and bss_start is None:
            bss_start = int(m.group(1), 16)
    both = sorted((k for k in decl if k in ours), key=lambda k: decl[k][0])
    if a.sizes:
        sizes(a, decl, ours, both, bss_start, dsyms)
        return
    placed = sum(1 for k in both if ours[k] == decl[k][0])
    steps, prev = [], None
    for k in both:
        dl = ours[k] - decl[k][0]
        if dl != prev:
            steps.append(k)
            prev = dl
    if not a.steps:
        od, osec, _ = exemeasure.sections(os.path.join(exemeasure.game_dir(), "GildeGold.exe"))
        ud, usec, _ = exemeasure.sections(exemeasure.OURS)
        for s in (".rdata", ".data"):
            va, vs, raw = osec[s]
            k = len(raw)
            while k and raw[k - 1] == 0:
                k -= 1
            vb, vsb, rawb = usec[s]
            kb = len(rawb)
            while kb and rawb[kb - 1] == 0:
                kb -= 1
            print("%-6s original: %d bytes up to the last nonzero byte, %d in all; ours: %d / %d" % (s, k, vs, kb, vsb))
        print("data symbols: %d with an original address, %d in our map, %d at their original address, %d steps" % (
            len(decl), len(both), placed, len(steps)))
        lost = [k for k in decl if k not in ours]
        if lost:
            print("not in our map (dropped or renamed): %d, e.g. %s" % (len(lost), " ".join(sorted(lost)[:8])))
    rows = [k for k in steps if (not a.section or decl[k][1] == a.section) and (not a.unit or decl[k][4] == a.unit)]
    print("steps%s (original address order; delta = ours - original):" % (
        "" if not (a.section or a.unit) else " in " + " ".join(x for x in (a.section, a.unit) if x)))
    for k in rows[:a.n]:
        at, sec, size, room, u, src = decl[k]
        flag = "  room>size" if room not in ("-", str(size)) else ""
        print("  0x%08x %-30s delta %+8d  %-5s unit=%-10s size=%s room=%s %s%s" % (
            at, k, ours[k] - at, sec, u, size, room, src, flag))


def sizes(a, decl, ours, both, bss_start, dsyms):
    """Initialised .data per unit: ours (every map symbol of the unit's obj up to our .bss) against the original's
    (each declared datum's room, the original's address up to its last nonzero .data byte)."""
    od, osec, _ = exemeasure.sections(os.path.join(exemeasure.game_dir(), "GildeGold.exe"))
    va, vs, raw = osec[".data"]
    k = len(raw)
    while k and raw[k - 1] == 0:
        k -= 1
    base = 0x400000  # the image base of both exes
    o_end = base + va + k              # the original's initialised .data ends here (the rest is .bss)
    u_end = base + va + bss_start      # ours: the map's .bss contribution
    print("initialised .data: the original 0x%08x..0x%08x (%d), ours ..0x%08x (%d), %+d" % (
        base + va, o_end, o_end - base - va, u_end, u_end - base - va, u_end - o_end))
    # obj -> unit by the declared symbols it defines
    objunit = {}
    for at, obj, name in dsyms:
        if name in decl:
            objunit.setdefault(obj, {}).setdefault(decl[name][4], 0)
            objunit[obj][decl[name][4]] += 1
    objunit = {o: max(c, key=c.get) for o, c in objunit.items()}
    dsyms.sort()
    ourper = {}
    for i, (at, obj, name) in enumerate(dsyms):
        if at >= u_end:
            break
        nxt = dsyms[i + 1][0] if i + 1 < len(dsyms) else u_end
        u = objunit.get(obj, obj.split(":")[0][:12])
        ourper[u] = ourper.get(u, 0) + min(nxt, u_end) - at
    order = sorted(decl, key=lambda n: decl[n][0])
    origper = {}
    for i, n in enumerate(order):
        at = decl[n][0]
        if decl[n][1] == "rdata" or at >= o_end or at < base + va:
            continue
        nxt = min(decl[order[i + 1]][0] if i + 1 < len(order) else o_end, o_end)
        origper[decl[n][4]] = origper.get(decl[n][4], 0) + nxt - at
    rows = sorted(set(ourper) | set(origper), key=lambda u: -abs(ourper.get(u, 0) - origper.get(u, 0)))
    print("per unit (ours - original; the original's bytes are its declared data's rooms, strings included):")
    for u in rows[:a.n]:
        print("  %-14s ours %7d  original %7d  %+7d" % (u, ourper.get(u, 0), origper.get(u, 0),
                                                       ourper.get(u, 0) - origper.get(u, 0)))
    wrong = [n for n in both if decl[n][0] >= o_end and ours[n] < u_end]
    print("initialised in ours, .bss in the original: %d symbols, %d bytes" % (len(wrong), sum(decl[n][2] for n in wrong)))
    for n in sorted(wrong, key=lambda n: -decl[n][2])[:a.n]:
        at, sec, size, room, u, src = decl[n]
        print("  0x%08x %-30s size=%-6d unit=%-10s %s" % (at, n, size, u, src))
    wrong = [n for n in both if base + va <= decl[n][0] < o_end and ours[n] >= u_end]
    print(".bss in ours, initialised in the original: %d symbols, %d bytes" % (len(wrong), sum(decl[n][2] for n in wrong)))
    for n in sorted(wrong, key=lambda n: -decl[n][2])[:a.n]:
        at, sec, size, room, u, src = decl[n]
        print("  0x%08x %-30s size=%-6d unit=%-10s %s" % (at, n, size, u, src))


if __name__ == "__main__":
    main()
