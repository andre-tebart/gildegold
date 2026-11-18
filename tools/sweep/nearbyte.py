"""nearbyte.py: the near-byte sweep list (near-byte sweep s22).

    uv run python tools/sweep/nearbyte.py [--max N] [--out FILE]

Runs `dcp grade UNIT` for every unit that has a function below byte (ledger/grades.tsv) and lists the shape and
draft functions whose distance (`dist='N ins / ...'`) is at most --max instructions (default 12), sorted by
instructions, then bytes. Tie-break-sensitive functions and known dead ends (EXCLUDE) are left out.
"""
import argparse, os, re, subprocess

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
EXCLUDE = {
    "ai_TutorialInitChapter2", "ai_TutorialInitChapter3", "he_PostEvent67", "main_RoundReport", "d3_MirDrawObject",
    "ai_FillEnemiesTab", "ord_CountGridObjects", "ls_ReadSaveHeader", "d3t_CreateSnowMipmaps",
    "d3t_MoveObjectPath", "ai_PokerPickStrategies",
}
LINE = re.compile(r"^fn=(\S+)\s+name=(\S+)\s+unit=(\S+)\s+experiment=(\S+).*?dist='(\d+) ins / (\d+) bytes")


def units_below():
    us = set()
    for l in open(os.path.join(ROOT, "ledger", "grades.tsv"), encoding="utf-8"):
        p = l.rstrip("\n").split("\t")
        if len(p) > 4 and p[0] == "exe" and p[4] != "byte":
            us.add(p[3])
    return sorted(us)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--max", type=int, default=12)
    ap.add_argument("--out")
    a = ap.parse_args()
    rows = []
    for u in units_below():
        r = subprocess.run(["dcp", "grade", u], cwd=ROOT, capture_output=True, text=True, encoding="utf-8",
                           errors="replace")
        for l in r.stdout.splitlines():
            m = LINE.match(l)
            if not m:
                continue
            addr, name, unit, exp, ins, byts = m.groups()
            if exp not in ("shape", "draft") or name in EXCLUDE or int(ins) > a.max:
                continue
            rows.append((int(ins), int(byts), name, unit, exp, addr))
    rows.sort()
    text = "".join("%d\t%d\t%s\t%s\t%s\t%s\n" % r for r in rows)
    if a.out:
        open(a.out, "w", encoding="utf-8").write(text)
    print(text, end="")
    print("# %d functions" % len(rows))


if __name__ == "__main__":
    main()
