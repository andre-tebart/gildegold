"""extorder: the order in which our link meets the library symbols: per object (link order) its new externals.

    uv run python tools/exe/extorder.py            # every object that brings a new non-game external
    uv run python tools/exe/extorder.py -n 30

Why: LINK 6 pulls library members in the order their symbols turn up in the unresolved list, and that list grows
in link order, object by object, each object's undefined externals in its COFF symbol-table order (cl writes
them per function in definition order; inside a function in reverse order of first use). So LIBC's member order
(.text, and its .data / .bss) follows this list. Compare it with the original's member order (`dcp answers
0x5de4a0..0x5eb000`, the unit=lib:LIBC.LIB/... column). Rules found with it (s10, notes/vc6-data.md):
- a member the original links after all others (strupr .. getcwd) is reached through OLDNAMES.LIB only: the
  game called the old name (`strupr`, not `_strupr`);
- a member the original pulls earlier than any surviving user explains is pulled by a function /OPT:REF removed
  (its externals stay in the object's symbol table): fpinit / ftol before udp's fprintf, rand before ch's sprintf,
  atof / crt0dat right after chc's strncpy, fwrite after sb's fread. Not reproducible without that function.
An old name shows with C's one underscore (`_strupr`, `_strcmpi`); the CRT's own is `__strupr` / `__strcmpi`.
Game symbols (prefix_Name), __imp_ imports and zlib are left out.
"""
import argparse, os, re, struct

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
GAME = re.compile(r"^_[a-z0-9]+_[A-Za-z0-9]|^__imp_|^_(adler32|crc32|deflate|inflate|gz|unz|zcalloc|zcfree|get_crc_table)")


def externals(path):
    b = open(path, "rb").read()
    symoff, nsym = struct.unpack_from("<II", b, 8)
    stro = symoff + nsym * 18
    out, i = [], 0
    while i < nsym:
        o = symoff + i * 18
        name = b[o:o + 8]
        if name[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", name, 4)[0]
            nm = b[stro + off:b.index(b"\0", stro + off)].decode("latin-1")
        else:
            nm = name.rstrip(b"\0").decode("latin-1")
        val, sec, typ, cls, naux = struct.unpack_from("<IhHBB", b, o + 8)
        if sec == 0 and cls == 2 and val == 0:  # undefined external, not a COMM
            out.append(nm)
        i += 1 + naux
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", type=int, default=0)
    a = ap.parse_args()
    rsp = os.path.join(ROOT, "build", "link", "exe", "link.rsp")
    objs = [l.strip() for l in open(rsp) if l.startswith("build/obj")]
    first = {}
    for l in open(os.path.join(ROOT, "build", "link", "exe", "exe.map")):
        p = l.split()
        if len(p) >= 4 and p[0].startswith("0001:") and p[-1].endswith(".obj"):
            first.setdefault(p[-1], p[1])
    seen, k = set(), 0
    for o in objs:
        ext = externals(os.path.join(ROOT, o))
        new = [n for n in ext if n not in seen and not GAME.match(n)]
        seen.update(ext)
        if new:
            print("%-30s %s" % (first.get(os.path.basename(o), os.path.basename(o)[:16]), " ".join(new)))
            k += 1
            if a.n and k >= a.n:
                break


if __name__ == "__main__":
    main()
