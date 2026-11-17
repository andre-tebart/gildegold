"""rawsizes.py [-n N]: each size-queue function's raw size on both sides (from dcp disasm, fill dropped),
smallest gaps worth picking first. Slow: two dcp disasm calls per function.
  python tools/exe/rawsizes.py -n 80"""
import re, subprocess, sys
N = sys.argv[sys.argv.index("-n") + 1] if "-n" in sys.argv else "300"
q = subprocess.run(["python", "tools/exe/exemeasure.py", "--queue", "--no-link", "-n", N], capture_output=True, text=True).stdout
rows = []
for l in q.splitlines():
    m = re.match(r"\s+0x([0-9a-f]+) (\S+)\s+unit=(\S+)\s+(\S+)\s+orig=(\d+) ours=(\d+)", l)
    if m: rows.append(m.groups())
def raw(args):
    out = subprocess.run(["dcp", "disasm"] + args, capture_output=True, text=True).stdout
    last = None; start = None
    for l in out.splitlines():
        m = re.match(r"\s+0x([0-9a-f]{8})\s+([0-9a-f]+)\s+(.*)", l)
        if m:
            a = int(m.group(1), 16)
            if start is None: start = a
            if "; fill" in l or m.group(3).startswith("int3"): continue
            last = a + len(m.group(2)) // 2
    return (last - start) if last else -1
for addr, name, unit, g, o, u in rows:
    ro = raw([name]); ru = raw(["--ours", name])
    print("%s %-34s %-12s %-6s orig=%s ours=%s rawo=%d rawu=%d d=%+d" % (addr, name, unit, g, o, u, ro, ru, ro - ru), flush=True)
