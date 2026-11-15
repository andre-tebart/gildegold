"""grep_orig: which functions of the original contain an instruction pattern, with their grades.

    python tools/search/grep_orig.py REGEX [--grade byte] [--unit am] [--multi] [-n 40]

Each function of ledger/grades.tsv is disassembled once (`dcp disasm NAME --no-bytes`, about 8 minutes for the
whole image the first time) into build/orig-asm/NAME.asm; later runs read the cache. REGEX is matched per
instruction line (without the address; written compact: `add esi,4`, `[esp+esi*4+0x90]`), or with --multi over the whole function text (`call A[\\s\\S]*call B`).
Use it for an odd instruction form: the byte functions that have it show in their source which construct
gives it. --refresh rebuilds the cache.
"""
import argparse, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
CACHE = os.path.join(ROOT, "build", "orig-asm")


def rows():
    out = []
    with open(os.path.join(ROOT, "ledger", "grades.tsv"), encoding="utf-8") as f:
        for l in f:
            p = l.rstrip("\n").split("\t")
            if len(p) >= 6 and p[0] != "target":
                out.append(dict(addr=p[1], name=p[2], unit=p[3], code=p[4], read=p[5]))
    return out


def asm(name, refresh):
    path = os.path.join(CACHE, name + ".asm")
    if refresh or not os.path.exists(path):
        r = subprocess.run(["dcp", "disasm", name, "--no-bytes"], cwd=ROOT, capture_output=True, text=True,
                           encoding="latin-1")
        text = "\n".join(re.sub(r"^\s+0x[0-9a-f]+\s+", "", l) for l in r.stdout.split("\n")
                         if re.match(r"^\s+0x[0-9a-f]+\s", l))
        with open(path, "w", encoding="latin-1") as f:
            f.write(text)
        return text
    return open(path, encoding="latin-1").read()


def compact(text):
    """`mov eax, dword ptr [esp + 8]` -> `mov eax,dword ptr [esp+8]`: patterns are written without those spaces"""
    return re.sub(r" *([,+*]) *| (-) ", r"\1\2", text)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("regex")
    ap.add_argument("--grade", help="only functions of this code grade (byte, shape, draft)")
    ap.add_argument("--unit")
    ap.add_argument("--multi", action="store_true", help="match over the whole function text")
    ap.add_argument("-n", type=int, default=40, help="functions listed (0: all)")
    ap.add_argument("--refresh", action="store_true")
    a = ap.parse_args()
    os.makedirs(CACHE, exist_ok=True)
    pat = re.compile(a.regex, re.S if a.multi else 0)
    todo = [r for r in rows() if (not a.grade or r["code"] == a.grade) and (not a.unit or r["unit"] == a.unit)]
    missing = sum(1 for r in todo if a.refresh or not os.path.exists(os.path.join(CACHE, r["name"] + ".asm")))
    if missing > 50:
        sys.stderr.write("disassembling %d functions into build/orig-asm (once)...\n" % missing)
    with ThreadPoolExecutor(4) as ex:
        texts = list(ex.map(lambda r: compact(asm(r["name"], a.refresh)), todo))
    hits = 0
    for r, text in zip(todo, texts):
        if a.multi:
            found = [m.group(0).split("\n")[0] for m in pat.finditer(text)]
        else:
            found = [l for l in text.split("\n") if pat.search(l)]
        if not found:
            continue
        hits += 1
        if a.n and hits > a.n:
            continue
        print("%s  %-34s %-6s %-6s %d  %s" % (r["addr"], r["name"], r["unit"], r["code"], len(found), found[0][:70]))
    print("%d functions of %d match" % (hits, len(todo)))


if __name__ == "__main__":
    main()
