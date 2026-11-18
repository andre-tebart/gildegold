"""blockusers: which data blocks each function of a unit refers to in the ORIGINAL code, in address order.

    python tools/exe/blockusers.py UNIT LO:HI:LABEL [LO:HI:LABEL ...]

For every function of UNIT (from `dcp grade UNIT`, address order) it scans the original's disassembly cache
(build/orig-asm/NAME.asm, written by tools/search/grep_orig.py; run that once first) for addresses in each
[LO, HI) range and prints the labels hit, with up to four addresses each:

    0x004cd3c0 cm_LobbyAddChat byte A(60c7b0,60c7b4,60c7b8) bss(6f0cd8)
    0x004cd9d0 ai_FindWeaponRecord byte B(60c7c0,60cb58)

Why (data layout s11): a unit's .data is one object's plain block followed by its string literals; a second plain
block after the strings is a new object. Label the plain block + run of each candidate object (and the unit's
.bss / common ranges) and read where the users change: the last user of one block and the first user of the next
bound the object boundary (cm: the fight block 0x60c7c0.. from ai_FindWeaponRecord, the dump block 0x60eae0.. from
ai_DumpPersonHead; svgrave / svtip at 0x587e00; menu / pokerwin at 0x5d9de0). A pooled literal can be read by a
later object too (cm's "$A"), so judge by each block's own plain data and its run's first users. Then check the
candidate points with tools/c2dag/bprobe.py before `dcp unit split`.
"""
import os
import re
import subprocess
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    unit = sys.argv[1]
    rngs = []
    for a in sys.argv[2:]:
        lo, hi, lab = a.split(":")
        rngs.append((int(lo, 16), int(hi, 16), lab))
    out = subprocess.run("dcp grade " + unit, shell=True, capture_output=True, text=True, cwd=ROOT).stdout
    for m in re.finditer(r"fn=(0x[0-9a-f]+)\s+name=(\S+)\s+unit=\S+\s+experiment=(\S+)", out):
        addr, name, grade = m.groups()
        p = os.path.join(ROOT, "build", "orig-asm", name + ".asm")
        if not os.path.exists(p):
            print(addr, name, grade, "(no build/orig-asm entry: run tools/search/grep_orig.py once)")
            continue
        hits = {}
        for v in re.findall(r"0x([0-9a-f]{6,8})\b", open(p, encoding="latin-1").read()):
            x = int(v, 16)
            for lo, hi, lab in rngs:
                if lo <= x < hi:
                    hits.setdefault(lab, set()).add(x)
        print(addr, name, grade, " ".join("%s(%s)" % (k, ",".join("%x" % y for y in sorted(v)[:4]))
                                          for k, v in sorted(hits.items())))


if __name__ == "__main__":
    main()
