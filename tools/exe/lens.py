"""lens.py ORIG_DISASM OURS_DISASM: where a size gap is, instruction by instruction (also jump lengths, which
hunksz.py misses when a short vs near jump is the whole gap). Aligns by mnemonic; prints each equal instruction of
another length and each differing run whose byte sum differs, then the total.
  dcp disasm NAME > o.txt; dcp disasm --ours NAME > u.txt; python tools/exe/lens.py o.txt u.txt"""
import re,sys,difflib
def rd(f):
    r=[]
    for l in open(f):
        m=re.match(r"\s+0x([0-9a-f]{8})\s+([0-9a-f]+)\s+(.*)",l)
        if m:
            ins=re.sub(r"0x[0-9a-f]+( <[^>]*>)?","X",m.group(3))
            r.append((ins.split()[0]+(' '+ins.split()[1] if ins.startswith('j') else ''),len(m.group(2))//2,m.group(1),m.group(3)))
    return r
o=rd(sys.argv[1]);u=rd(sys.argv[2])
# align by mnemonic sequence
sm=difflib.SequenceMatcher(None,[x[0] for x in u],[x[0] for x in o],autojunk=False)
tot=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        for a,b in zip(u[i1:i2],o[j1:j2]):
            if a[1]!=b[1]: print("len",a[2],a[3],a[1],"|",b[2],b[3],b[1]); tot+=b[1]-a[1]
    else:
        su=sum(x[1] for x in u[i1:i2]); so=sum(x[1] for x in o[j1:j2])
        if su!=so: print(tag,"ours",[x[3] for x in u[i1:i2]][:6],su,"orig",[x[3] for x in o[j1:j2]][:6],so); tot+=so-su
print("total orig-ours",tot)
