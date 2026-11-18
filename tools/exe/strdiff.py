"""strdiff: the strings of an original .data / .rdata range against ours, as a sequence diff.

    uv run python tools/exe/strdiff.py ORIG_LO ORIG_HI OURS_LO OURS_HI

Reads the NUL-terminated strings (2+ printable bytes) of [ORIG_LO, ORIG_HI) in the original exe and of
[OURS_LO, OURS_HI) in build/link/exe/GildeGold.exe (pick OURS_LO / HI from build/link/exe/exe.map: the same
units' first and last symbols) and prints every non-equal run of difflib's opcodes, with both sides' addresses.

Why: a unit's .data is its plain block (named initialised globals, in definition order) followed by its string
literals (COMDATs, in function order; /GF pools an identical literal into the FIRST object in link order that
uses it). The diff shows at a glance:
- a string the original has in a unit's string run that ours has as a named char array in the plain block
  (`delete` on the original side, the name elsewhere): make it the literal at its users (s10: main_HomeSweetHome,
  lcmap's news keys, lcamt's form names);
- a string the original has in an earlier unit's run whose surviving users are all later units: a literal of a
  function /OPT:REF removed from the earlier unit (s10: "dummy_Kartentisch_RECHTS_UNTEN" in main's run, used by lc);
- the same strings in another order: the functions that use them, or which unit uses them first.
"""
import os, re, struct, sys, difflib

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import holes  # noqa: E402

OURS = os.path.join(ROOT, "build", "link", "exe", "GildeGold.exe")


def read(img, lo, hi):
    pe = struct.unpack_from("<I", img, 0x3c)[0]
    n = struct.unpack_from("<H", img, pe + 6)[0]
    opt = struct.unpack_from("<H", img, pe + 20)[0]
    for i in range(n):
        o = pe + 24 + opt + 40 * i
        va, raw, ptr = struct.unpack_from("<III", img, o + 12)
        va += 0x400000
        if va <= lo < va + raw:
            return img[ptr + lo - va: ptr + hi - va]
    sys.exit("0x%x is in no section with raw data" % lo)


def strings(b, base):
    return [(base + m.start(), m.group()[:-1].decode("latin-1")) for m in re.finditer(rb"[\x20-\xff]{2,}\x00", b)]


def main():
    if len(sys.argv) != 5:
        sys.exit(__doc__)
    olo, ohi, ulo, uhi = (int(x, 16) for x in sys.argv[1:])
    a = strings(read(holes.image(), olo, ohi), olo)
    b = strings(read(open(OURS, "rb").read(), ulo, uhi), ulo)
    sm = difflib.SequenceMatcher(a=[s for _, s in a], b=[s for _, s in b], autojunk=False)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op != "equal":
            print(op, ["0x%x %r" % x for x in a[i1:i2]], "||", ["0x%x %r" % x for x in b[j1:j2]])


if __name__ == "__main__":
    main()
