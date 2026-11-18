"""drift.py FUNC: where along FUNC our code drifts from the original in bytes (from dcp asmdiff --all --lines
and dcp disasm): prints each point where (our address - original address) changes, with the source line, so a size
difference can be pinned to the statement that grows or shrinks (size queue s13)."""
import subprocess, sys, re
name = sys.argv[1]
a = subprocess.run(["dcp", "asmdiff", name, "--all", "--lines"], capture_output=True, text=True).stdout.splitlines()
o = subprocess.run(["dcp", "disasm", name], capture_output=True, text=True).stdout.splitlines()
orig = [int(m.group(1), 16) for m in (re.match(r"\s+0x([0-9a-f]+)\s+[0-9a-f]+\s", l) for l in o) if m]
oi = 0; last = None
for l in a:
    m = re.match(r"([ ~!+-])\s+0x([0-9a-f]+)\s+(.*)", l)
    if not m: continue
    k, ad, txt = m.group(1), int(m.group(2), 16), m.group(3)
    if k == '+':
        oi += 1; continue
    if k == '-': continue
    d = ad - orig[oi]; oi += 1
    if d != last:
        print("ours %x orig %x drift %+d  %s" % (ad, orig[oi-1], d, txt[:70])); last = d
