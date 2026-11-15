/* c2.dll proxy for VC6 SP5+PP (C2 13.00.9044): loads the real back end (c2real.dll, same directory), hooks its
   scheduling-priority pass (dag.c, RVA 0x14f6a) and writes every basic block's scheduling graph to the file named
   by the environment variable C2DAG_OUT: the instructions in their order before scheduling, each with its
   priority, height, in/out degree, flags and its dependency edges (target/latency:kind).
   Build: cl /nologo /LD /O1 c2proxy.c /Fec2.dll /link /BASE:0x20000000 */
#include <windows.h>
#include <stdio.h>

typedef int (__stdcall *INVOKE)(void *, void *, void *);
typedef int (__stdcall *ABORT)(void *);

static HMODULE real;
static INVOKE realInvoke;
static ABORT realAbort;
static FILE *out;
static int blockNo;
static unsigned char *tramp;
static void *savedP;
static void *retStack[64], *pStack[64], *retStack2[64];   /* the hooked passes recurse: one return address per level */
static int retTop, retTop2;

#define RVA_PRIORITY 0x14f6a
#define U32(p, o) (*(unsigned long *)((char *)(p) + (o)))
#define U16(p, o) (*(unsigned short *)((char *)(p) + (o)))
#define U8(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define PTR(p, o) (*(void **)((char *)(p) + (o)))

/* one operand list of a tuple (uses at +0x1c, defs at +0x20), as tools c2prio reads them: kind byte at +8
   (1 register / symbol reference via +0x18 -> +8, 2 memory via +0x14 -> +8, 7 immediate, 10 a call's kill set),
   flag bytes at +0x10 / +0x11 (second byte: 10 last use). A symbol inside the back end's register table
   (0xc49e8, 0x58 bytes each) prints as the register, any other as its name (+0x18) or #id (+0x1c). */
static void opnds(void *o)
{
    static const char *regs[] = { "r0", "r1", "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
    int g = 0;
    for (; o && g < 64; o = PTR(o, 0), g++) {
        unsigned kind = U8(o, 8);
        void *ref = kind == 1 ? PTR(o, 0x18) : kind == 2 ? PTR(o, 0x14) : 0, *sym = 0;
        fprintf(out, " ");
        if (ref && !IsBadReadPtr(ref, 12))
            sym = PTR(ref, 8);
        if (sym && !IsBadReadPtr(sym, 0x20)) {
            unsigned long d = (unsigned long)sym - ((unsigned long)real + 0xc49e8);
            char *nm = PTR(sym, 0x18);
            if (d < 0x58 * 80 && d % 0x58 == 0) {
                if (d / 0x58 < 10)
                    fprintf(out, "%s%s", kind == 2 ? "m:" : "", regs[d / 0x58]);
                else
                    fprintf(out, "%sr%lu", kind == 2 ? "m:" : "", d / 0x58);
            } else if (nm && !IsBadStringPtrA(nm, 96) && nm[0] > 32 && nm[0] < 127)
                fprintf(out, "%s%s", kind == 2 ? "m:" : "", nm);
            else
                fprintf(out, "%s#%lx", kind == 2 ? "m:" : "s", U32(sym, 0x1c));
        } else
            fprintf(out, "k%x", kind);
        fprintf(out, "/%x.%x", U8(o, 0x10), U8(o, 0x11));
    }
}

static void cname(void *c, char *buf)
{
    void *c0 = PTR(c, 0), *s;
    char *nm = 0;
    if (IsBadReadPtr(c0, 0x20)) {
        strcpy(buf, "?");
        return;
    }
    s = PTR(c0, 0);
    if (!IsBadReadPtr(s, 0x1c))
        nm = PTR(s, 0x18);
    if (nm && !IsBadStringPtrA(nm, 96) && (nm[0] == '_' || (nm[0] >= 'A' && nm[0] <= 'z')))
        sprintf(buf, "%.80s@%lx", nm, (unsigned long)c & 0xffff);
    else
        sprintf(buf, "%s#%lx@%lx", U32(c0, 4) == 0x100d ? "const" : U32(c0, 4) == 3 ? "temp" : "k", U32(c0, 0x1c),
                (unsigned long)c & 0xffff);
}

/* mid-function hooks of the allocator's priority pass (sites from rea's c2prio): the handler gets the saved
   registers r[0..8] = eflags, edi, esi, ebp, esp, ebx, edx, ecx, eax */
static int allocFirst, allocBlk;
static void __cdecl hBlock(unsigned long *r)
{
    void *b, *t, *end;
    long lo = 0x7fffffff, hi = -1;
    int g = 0;
    if (!out || !allocFirst)
        return;
    b = *(void **)(r[4] + 0x1c);
    if (!IsBadReadPtr(b, 0x24)) {
        end = PTR(b, 0x20);
        for (t = PTR(b, 0x1c); t && t != end && g < 5000 && !IsBadReadPtr(t, 0x14); t = PTR(t, 0), g++) {
            long ln = *(short *)((char *)t + 0x10);
            if (ln > 0 && ln < lo) lo = ln;
            if (ln > hi) hi = ln;
        }
    }
    fprintf(out, " ablock %d lines %ld-%ld\n", allocBlk++, hi < 0 ? 0 : lo, hi < 0 ? 0 : hi);
}
static void __cdecl hUse(unsigned long *r)
{
    char nm[128];
    if (!out || !allocFirst)
        return;
    cname((void *)r[2], nm);
    fprintf(out, " ref use blk %d line %d %s cost %ld\n", allocBlk, (int)*(short *)(r[5] + 0x10), nm, (long)r[1]);
}
static void __cdecl hDef(unsigned long *r)
{
    char nm[128];
    if (!out || !allocFirst)
        return;
    cname((void *)r[2], nm);
    fprintf(out, " ref def blk %d line %d %s cost %ld\n", allocBlk, (int)*(short *)(r[1] + 0x10), nm, (long)r[8]);
}
static void __cdecl hP(unsigned long *r)
{
    if (!out || !allocFirst)
        return;
    fprintf(out, " P blk %d = %ld x %ld\n", allocBlk - 1, (long)r[2], *(long *)(r[4] + 0x24));
}
/* cross-jumping (fg.c, FUN_10712437): called with a `jmp L` tuple; it compares up to 30 instructions before the
   jmp with up to 30 before the label L and merges what can be sunk to the end of both. Print both regions. */
static void xjRegion(void *t, void *other)
{
    int g = 0;
    void *p = t;
    for (; g < 30; g++) {
        unsigned cls;
        p = PTR(p, 0xc);
        if (!p || IsBadReadPtr(p, 0x24))
            break;
        cls = U8(p, 8);
        if ((cls == 0x16 && U32(p, 4) != 0x2bd) || p == other || cls == 0x17 || cls == 0x19)
            break;
        fprintf(out, " %lx@%d", U32(p, 4), (int)*(short *)((char *)p + 0x10));
    }
}
static void __cdecl hXJ(unsigned long *r)
{
    void *j = (void *)r[7], *lab = 0, *o;
    char *cur, *nm = 0;
    if (!out || IsBadReadPtr(j, 0x24))
        return;
    o = PTR(j, 0x1c);
    if (o && !IsBadReadPtr(o, 0x18) && !IsBadReadPtr(PTR(o, 0x14), 0x34))
        lab = PTR(PTR(o, 0x14), 0x30);
    if (!lab || IsBadReadPtr(lab, 0x24))
        return;
    cur = *(char **)((char *)real + 0xad730);
    if (!IsBadReadPtr(cur, 0x1c))
        nm = *(char **)(cur + 0x18);
    fprintf(out, "xjump %s jmp@%d\n  before the jmp (last first):", nm && !IsBadStringPtrA(nm, 128) ? nm : "?",
            (int)*(short *)((char *)j + 0x10));
    xjRegion(j, lab);
    fprintf(out, "\n  before the label (last first):");
    xjRegion(lab, j);
    fprintf(out, "\n");
}
/* tail merging after allocation (fg.c, the flow pass FUN_1070a5fe run with merging on from FUN_1070ec81): every
   merge ends in FUN_1071355b(first, last of the copy that is deleted, first, last of the copy that is kept); its
   return address names the merger. `tmerge FUNC by WHO` then both runs as opcode@line, first to last.
     xjump  FUN_10712437  a `jmp L` against the code falling into L (graph match, up to 30 instructions)
     pair   FUN_1074633c  two `jmp L` of one label L (linear match from the jmps backwards; the later reference
                          of L's list against an earlier one)
     move   FUN_107711fb  two `jmp L` when nothing falls into L: the kept copy is moved in front of L
     ret    FUN_1072a413  return-value merging (only when [c2+0xc45a0] == 0, i.e. not under /O2)
   `ttry FUNC pair|move jmp@A jmp@B` is one attempt of the two linear mergers (A = the copy deleted by default). */
static void runText(void *a, void *b)
{
    int g = 0;
    void *p = a;
    for (; p && g < 64 && !IsBadReadPtr(p, 0x24); p = PTR(p, 0), g++) {
        fprintf(out, " %lx@%d", U32(p, 4), (int)*(short *)((char *)p + 0x10));
        if (p == b)
            return;
    }
    fprintf(out, " ..");
}
static const char *curName(void)
{
    char *cur = *(char **)((char *)real + 0xad730), *nm = 0;
    if (!IsBadReadPtr(cur, 0x1c))
        nm = *(char **)(cur + 0x18);
    return nm && !IsBadStringPtrA(nm, 128) ? nm : "?";
}
static void __cdecl hMerge(unsigned long *r)
{
    unsigned long *sp = (unsigned long *)r[4], ra;
    const char *who;
    if (!out || IsBadReadPtr(sp, 0x10))
        return;
    ra = sp[0] - (unsigned long)real;
    who = ra == 0x126b2 ? "xjump" : ra == 0x4660d ? "pair" : ra == 0x712ff ? "move" : ra == 0x76887 ? "ret" : "?";
    fprintf(out, "tmerge %s by %s o2=%lu\n  deleted:", curName(), who, U32(real, 0xc45a0));
    runText((void *)r[7], (void *)r[6]);
    fprintf(out, "\n  kept:   ");
    runText((void *)sp[1], (void *)sp[2]);
    fprintf(out, "\n");
}
static void __cdecl hTry(unsigned long *r, const char *who)
{
    void *a = (void *)r[7], *b = (void *)r[6], *o, *lab = 0;
    if (!out || IsBadReadPtr(a, 0x24) || IsBadReadPtr(b, 0x24))
        return;
    o = PTR(a, 0x1c);
    if (o && !IsBadReadPtr(o, 0x18) && U8(o, 8) == 4 && !IsBadReadPtr(PTR(o, 0x14), 0x34))
        lab = PTR(PTR(o, 0x14), 0x30);
    fprintf(out, "ttry %s %s jmp@%d jmp@%d L%04lx\n", curName(), who, (int)*(short *)((char *)a + 0x10),
            (int)*(short *)((char *)b + 0x10), (unsigned long)lab & 0xffff);
    if (getenv("C2DAG_TRYDUMP")) {
        void *s[2];
        int k, g;
        s[0] = a; s[1] = b;
        for (k = 0; k < 2; k++) {
            void *p = s[k];
            for (g = 0; g < 6; g++) {
                p = PTR(p, 0xc);
                if (!p || IsBadReadPtr(p, 0x28))
                    break;
                fprintf(out, "   %c %lx %x %d x24 %lx x28 %lx fl %x |", "AB"[k], U32(p, 4), U8(p, 8), (int)*(short *)((char *)p + 0x10), U32(p, 0x24), U32(p, 0x28),
                        U16(p, 0xa));
                if (U8(p, 9) & 1) {
                    __try {
                        opnds(PTR(p, 0x1c));
                        fprintf(out, " |");
                        opnds(PTR(p, 0x20));
                    } __except (1) {
                        fprintf(out, " (fault)");
                    }
                }
                fprintf(out, "\n");
            }
        }
    }
}
/* the IL of the function named by C2DAG_IL (plain name) at three points: `pre` before the first flow pass after
   allocation (FUN_1070ea02), `merge` before the merging flow pass (FUN_1070ec81), `post` after it. One line per
   label (`Lxxxx:` and its number of references) and per instruction (opcode class line, a jump's target label). */
static int ilWanted(void)
{
    const char *w = getenv("C2DAG_IL"), *nm = curName();
    size_t n;
    if (!w || !*w)
        return 0;
    if (*nm == '_' || *nm == '@')
        nm++;
    n = strlen(w);
    return !strncmp(nm, w, n) && (nm[n] == 0 || nm[n] == '@');
}
static void ilDump(unsigned long fn, const char *stage)
{
    void *x, *t, *end;
    int g = 0;
    if (!out || !ilWanted() || IsBadReadPtr((void *)fn, 0x10))
        return;
    x = PTR(fn, 8);
    if (IsBadReadPtr(x, 8) || IsBadReadPtr(PTR(x, 0), 0x20) || IsBadReadPtr(PTR(x, 4), 0x24))
        return;
    t = PTR(PTR(x, 0), 0x1c);
    end = PTR(PTR(x, 4), 0x20);
    if (IsBadReadPtr(t, 4))
        return;
    fprintf(out, "til %s %s\n", curName(), stage);
    for (t = PTR(t, 0); t && t != end && g < 20000 && !IsBadReadPtr(t, 0x28); t = PTR(t, 0), g++) {
        unsigned cls = U8(t, 8);
        if (cls == 0x1a) {
            int n = 0;
            void *r = PTR(t, 0x1c);
            for (; r && n < 1000 && !IsBadReadPtr(r, 4); r = PTR(r, 0))
                n++;
            fprintf(out, " L%04lx: refs %d\n", (unsigned long)t & 0xffff, n);
        } else if (U8(t, 9) & 1) {
            void *o = PTR(t, 0x1c), *lab = 0;
            fprintf(out, "   %lx %x %d", U32(t, 4), cls, (int)*(short *)((char *)t + 0x10));
            if (cls == 0x11 && o && !IsBadReadPtr(o, 0x18) && U8(o, 8) == 4 && !IsBadReadPtr(PTR(o, 0x14), 0x34))
                lab = PTR(PTR(o, 0x14), 0x30);
            if (lab)
                fprintf(out, " -> L%04lx%s", (unsigned long)lab & 0xffff, U32(t, 0x24) ? " cond" : "");
            else {
                fprintf(out, " |");
                opnds(PTR(t, 0x1c));
                fprintf(out, " |");
                opnds(PTR(t, 0x20));
            }
            fprintf(out, "\n");
        }
    }
}
static void __cdecl hIlPre(unsigned long *r) { ilDump(r[2], "pre"); }
static void __cdecl hIlMerge(unsigned long *r) { ilDump(r[2], "merge"); }
static void __cdecl hIlPost(unsigned long *r) { ilDump(r[2], "post"); }
static void __cdecl hTryPair(unsigned long *r) { hTry(r, "pair"); }
/* FUN_1074633c after its compare loop: the number of equal instructions and the first unequal pair */
static void __cdecl hPairCount(unsigned long *r)
{
    void *a = (void *)r[2], *b = (void *)r[5];
    if (!out || !getenv("C2DAG_TRYDUMP") || IsBadReadPtr(a, 0x28) || IsBadReadPtr(b, 0x28))
        return;
    fprintf(out, "  pair equal %lu, stop at %lx@%d / %lx@%d\n", *(unsigned long *)(r[4] + 0x20), U32(a, 4),
            (int)*(short *)((char *)a + 0x10), U32(b, 4), (int)*(short *)((char *)b + 0x10));
}
static void __cdecl hTryMove(unsigned long *r) { hTry(r, "move"); }
/* FUN_1074633c's size test: the bytes of the equal run (counted up to 22; merged only above 22, or 2 when
   [c2+0xc45a0] is 0) */
static void __cdecl hPairSize(unsigned long *r)
{
    if (out && getenv("C2DAG_TRYDUMP"))
        fprintf(out, "  pair bytes %lu\n", r[3]);
}
static void cands(void);
static void __cdecl hReg(unsigned long *r)
{
    char *cur, *nm = 0;
    if (!out)
        return;
    cur = *(char **)((char *)real + 0xad730);
    if (!IsBadReadPtr(cur, 0x1c))
        nm = *(char **)(cur + 0x18);
    fprintf(out, "allocreg %s\n", nm && !IsBadStringPtrA(nm, 128) ? nm : "?");
    cands();
}
static void midhook(unsigned long rva, int len, void (__cdecl *h)(unsigned long *), const unsigned char *expect)
{
    unsigned char *fn = (unsigned char *)real + rva, *p, *s;
    DWORD old;
    if (memcmp(fn, expect, len))
        return;
    s = p = (unsigned char *)VirtualAlloc(0, 64, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    *p++ = 0x60; *p++ = 0x9c; *p++ = 0x54;            /* pushad; pushfd; push esp */
    *p++ = 0xe8; *(long *)p = (long)h - (long)(p + 4); p += 4;
    *p++ = 0x83; *p++ = 0xc4; *p++ = 0x04;            /* add esp,4 */
    *p++ = 0x9d; *p++ = 0x61;                          /* popfd; popad */
    if (fn[0] == 0xe8 && len == 5) {                   /* a call rel32: retarget */
        *p++ = 0xe8; *(long *)p = (long)(fn + 5 + *(long *)(fn + 1)) - (long)(p + 4); p += 4;
    } else {
        memcpy(p, fn, len); p += len;
    }
    *p++ = 0xe9; *(long *)p = (long)(fn + len) - (long)(p + 4);
    VirtualProtect(fn, len, PAGE_EXECUTE_READWRITE, &old);
    fn[0] = 0xe9; *(long *)(fn + 1) = (long)s - (long)(fn + 5);
    memset(fn + 5, 0x90, len - 5);
    VirtualProtect(fn, len, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, len);
}

/* the register allocator's candidates of the current function (layout from rea's c2prio: hash buckets at
   0xac5b8..0xad5b8, next at +0x2c, priority at +0xc, tie at +0x44, register symbol at +0x10) */
static void cands(void)
{
    static const char *regs[] = { "r0", "r1", "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
    unsigned long a;
    for (a = 0xac5b8; a < 0xad5b8; a += 4) {
        void *c = *(void **)((char *)real + a);
        int g = 0;
        for (; c && !IsBadReadPtr(c, 0x48) && g < 5000; c = PTR(c, 0x2c), g++) {
            void *c0 = PTR(c, 0), *s;
            char *nm = 0;
            unsigned long r = U32(c, 0x10), d;
            if (IsBadReadPtr(c0, 0x20))
                continue;
            s = PTR(c0, 0);
            if (!IsBadReadPtr(s, 0x1c))
                nm = PTR(s, 0x18);
            fprintf(out, " cand ");
            if (nm && !IsBadStringPtrA(nm, 96) && (nm[0] == '_' || (nm[0] >= 'A' && nm[0] <= 'z')))
                fprintf(out, "%s", nm);
            else
                fprintf(out, "%s#%lx", U32(c0, 4) == 0x100d ? "const" : U32(c0, 4) == 3 ? "temp" : "k", U32(c0, 0x1c));
            d = r - ((unsigned long)real + 0xc49e8);
            fprintf(out, "@%lx prio %ld tie %lx reg %s", (unsigned long)c & 0xffff, (long)U32(c, 0xc), U32(c, 0x44),
                    r && d % 0x58 == 0 && d / 0x58 < 10 ? regs[d / 0x58] : "-");
            if (getenv("C2DAG_RAW")) {
                int j;
                fprintf(out, "  c0:");
                for (j = 0; j < 0x38; j += 4)
                    fprintf(out, " %lx", U32(c0, j));
                fprintf(out, "  c:");
                for (j = 0; j < 0x48; j += 4)
                    fprintf(out, " %lx", U32(c, j));
            }
            fprintf(out, "\n");
        }
    }
}

static void find(const char *name)
{
    /* discovery aid: which global of the back end leads to the current function's name */
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)real;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)real + dos->e_lfanew);
    IMAGE_SECTION_HEADER *s = IMAGE_FIRST_SECTION(nt);
    int k, o;
    unsigned long a;
    for (k = 0; k < nt->FileHeader.NumberOfSections; k++, s++) {
        if (!(s->Characteristics & IMAGE_SCN_MEM_WRITE))
            continue;
        for (a = 0; a + 4 <= s->Misc.VirtualSize; a += 4) {
            char *p = *(char **)((char *)real + s->VirtualAddress + a);
            if (IsBadReadPtr(p, 0x44))
                continue;
            if (!IsBadStringPtrA(p, 64) && !strcmp(p, name))
                fprintf(out, "  find %s: [rva %lx] -> string\n", name, s->VirtualAddress + a);
            for (o = 0; o < 0x40; o += 4) {
                char *q = *(char **)(p + o);
                if (!IsBadReadPtr(q, 8) && !IsBadStringPtrA(q, 64) && !strcmp(q, name))
                    fprintf(out, "  find %s: [rva %lx] -> +%x -> string\n", name, s->VirtualAddress + a, o);
            }
        }
    }
}

static void dump0(void)
{
    void *first, *last, *n, *e, *t;
    static void *nodes[4096];           /* static: the compiler's own stack is deep here */
    int cnt = 0, i, j;

    if (!out)
        return;
    first = PTR(savedP, 0);
    last = PTR(savedP, 4);
    for (n = first; n && cnt < 4096; n = PTR(n, 0)) {
        nodes[cnt++] = n;
        if (n == last)
            break;
    }
    {
        /* the function being compiled: [c2 + 0xad730] -> +0x18 -> its decorated name (found with C2DAG_FIND) */
        char *cur = *(char **)((char *)real + 0xad730), *nm = 0;
        if (!IsBadReadPtr(cur, 0x1c))
            nm = *(char **)(cur + 0x18);
        {
            static void *lastfn;
            if (cur != lastfn) {
                lastfn = cur;
                fprintf(out, "func %s\n", nm && !IsBadStringPtrA(nm, 128) ? nm : "?");
            }
        }
        fprintf(out, "block %d func %s nodes %d\n", blockNo++, nm && !IsBadStringPtrA(nm, 128) ? nm : "?", cnt);
    }
    {
        const char *f = getenv("C2DAG_FIND");
        char key[16], *hit;
        if (f) {
            sprintf(key, "%d:", blockNo - 1);
            hit = strstr(f, key);
            if (hit && (hit == f || hit[-1] == ',')) {
                char name[64];
                int m = 0;
                hit += strlen(key);
                while (hit[m] && hit[m] != ',' && m < 63) { name[m] = hit[m]; m++; }
                name[m] = 0;
                find(name);
            }
        }
    }
    for (i = 0; i < cnt; i++) {
        n = nodes[i];
        t = PTR(n, 0x1c);
        fprintf(out, " %3d op=%-4lx cls=%-2x ln=%-3ld prio=%-6lu h=%-3u in=%u out=%u fl=%02x ->", i, t ? U32(t, 4) : 0xffffUL,
                t ? U8(t, 8) : 0xff, t ? (long)U32(t, 0x10) : -1L, U32(n, 0x28), U16(n, 0x34), U16(n, 0x20), U16(n, 0x22), U8(n, 0x3c));
        for (e = PTR(n, 0xc); e; e = PTR(e, 0)) {
            void *tg = PTR(e, 0xc);
            for (j = 0; j < cnt && nodes[j] != tg; j++)
                ;
            fprintf(out, " %d/%u:%lx", j < cnt ? j : -1, U16(e, 0x14), U32(e, 0x10));
        }
        if (t) {
            fprintf(out, "  |");
            opnds(PTR(t, 0x1c));
            fprintf(out, " |");
            opnds(PTR(t, 0x20));
        }
        if (t && getenv("C2DAG_RAW")) {
            fprintf(out, "   raw");
            for (j = 0; j < 0x40; j += 4)
                fprintf(out, " %lx", U32(t, j));
        }
        fprintf(out, "\n");
    }
    fflush(out);
}

/* the dump only reads the back end's structures; a node shape it does not know (seen on a block built from two
   gotos into one statement) must not take the compiler down with it (C1001) */
static void __cdecl dump(void)
{
    __try {
        dump0();
    } __except (1) {
        if (out) {
            fprintf(out, " (dump fault: block skipped)\n");
            fflush(out);
        }
    }
}

/* second hook: the allocator's priority pass (FUN_10707a0b), first return per function */static unsigned char *tramp2;
static void __cdecl dump2(void)
{
    static void *lastfn;
    char *cur, *nm = 0;
    if (!out)
        return;
    cur = *(char **)((char *)real + 0xad730);
    if (cur == lastfn)
        return;
    lastfn = cur;
    allocFirst = 0;
    if (!IsBadReadPtr(cur, 0x1c))
        nm = *(char **)(cur + 0x18);
    fprintf(out, "alloc %s\n", nm && !IsBadStringPtrA(nm, 128) ? nm : "?");
    cands();
    fflush(out);
}

static void __cdecl enter2(void)
{
    static void *seen;
    void *cur = *(void **)((char *)real + 0xad730);
    allocFirst = cur != seen;
    seen = cur;
    allocBlk = 0;
    if (allocFirst && out) {
        char *nm = IsBadReadPtr(cur, 0x1c) ? 0 : *(char **)((char *)cur + 0x18);
        fprintf(out, "allocrefs %s\n", nm && !IsBadStringPtrA(nm, 128) ? nm : "?");
    }
}

static __declspec(naked) void stub2(void)
{
    __asm {
        pushad
        call enter2
        popad
        pop eax                         ; the return address goes on a stack of its own: the pass is re-entered
        push ecx
        mov ecx, retTop2
        mov dword ptr retStack2[ecx*4], eax
        inc ecx
        mov retTop2, ecx
        pop ecx
        call dword ptr [tramp2]
        push eax                        ; slot for the return address
        pushad
        mov ecx, retTop2
        dec ecx
        mov retTop2, ecx
        mov eax, dword ptr retStack2[ecx*4]
        mov [esp+32], eax
        call dump2
        popad
        ret
    }
}

static __declspec(naked) void stub(void)
{
    __asm {
        pop eax                         ; return address and the pass's argument (ecx) per nesting level
        push ecx
        mov ecx, retTop
        mov dword ptr retStack[ecx*4], eax
        pop eax
        mov dword ptr pStack[ecx*4], eax
        inc ecx
        mov retTop, ecx
        mov ecx, eax
        call dword ptr [tramp]
        push eax                        ; slot for the return address
        pushad
        mov ecx, retTop
        dec ecx
        mov retTop, ecx
        mov eax, dword ptr pStack[ecx*4]
        mov savedP, eax
        mov eax, dword ptr retStack[ecx*4]
        mov [esp+32], eax
        call dump
        popad
        ret
    }
}

static void install(void)
{
    char path[MAX_PATH], *s;
    unsigned char *fn;
    DWORD old;
    const char *o;

    GetModuleFileNameA(GetModuleHandleA("c2.dll"), path, MAX_PATH);
    s = strrchr(path, '\\');
    strcpy(s ? s + 1 : path, "c2real.dll");
    real = LoadLibraryA(path);
    if (!real) {
        MessageBoxA(0, path, "c2proxy: cannot load", 0);
        ExitProcess(3);
    }
    realInvoke = (INVOKE)GetProcAddress(real, "_InvokeCompilerPass@12");
    realAbort = (ABORT)GetProcAddress(real, "_AbortCompilerPass@4");
    o = getenv("C2DAG_OUT");
    if (o && *o)
        out = fopen(o, "a");
    fn = (unsigned char *)real + RVA_PRIORITY;
    if (fn[0] != 0x51 || fn[1] != 0x53 || fn[2] != 0x55 || fn[3] != 0x8b || fn[4] != 0x29)
        return; /* not the build this was written for: run unhooked */
    tramp = (unsigned char *)VirtualAlloc(0, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(tramp, fn, 5);
    tramp[5] = 0xe9;
    *(long *)(tramp + 6) = (long)(fn + 5) - (long)(tramp + 10);
    VirtualProtect(fn, 5, PAGE_EXECUTE_READWRITE, &old);
    fn[0] = 0xe9;
    *(long *)(fn + 1) = (long)stub - (long)(fn + 5);
    VirtualProtect(fn, 5, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 5);
    /* FUN_10707a0b: sub esp,28h / push esi / mov esi,ecx (6 bytes) */
    fn = (unsigned char *)real + 0x7a0b;
    if (fn[0] == 0x83 && fn[1] == 0xec && fn[2] == 0x28 && fn[3] == 0x56 && fn[4] == 0x8b && fn[5] == 0xf1) {
        tramp2 = (unsigned char *)VirtualAlloc(0, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        memcpy(tramp2, fn, 6);
        tramp2[6] = 0xe9;
        *(long *)(tramp2 + 7) = (long)(fn + 6) - (long)(tramp2 + 11);
        VirtualProtect(fn, 6, PAGE_EXECUTE_READWRITE, &old);
        fn[0] = 0xe9;
        *(long *)(fn + 1) = (long)stub2 - (long)(fn + 5);
        fn[5] = 0x90;
        VirtualProtect(fn, 6, old, &old);
        FlushInstructionCache(GetCurrentProcess(), fn, 6);
        {
            static const unsigned char e1[] = { 0xe8, 0xc9, 0x0b, 0x00, 0x00 };
            static const unsigned char e2[] = { 0x8b, 0x4e, 0x24, 0x03, 0xd7 };
            static const unsigned char e3[] = { 0x41, 0x89, 0x4e, 0x24, 0x8b, 0x4e, 0x3c };
            static const unsigned char e4[] = { 0x0f, 0xaf, 0x74, 0x24, 0x24 };
            static const unsigned char e5[] = { 0x81, 0xec, 0x8c, 0x00, 0x00, 0x00 };
            static const unsigned char e6[] = { 0xe8, 0xd8, 0x1b, 0xfd, 0xff };
            midhook(0x7b6e, 5, hP, e4);
            midhook(0x12437, 6, hXJ, e5);
            midhook(0x315c3, 5, hReg, e6);
            midhook(0x7b31, 5, hBlock, e1);
            midhook(0x7c42, 5, hUse, e2);
            midhook(0x7ed2, 7, hDef, e3);
        }
        {
            static const unsigned char m1[] = { 0x83, 0xec, 0x08, 0x85, 0xd2 };   /* FUN_1071355b */
            static const unsigned char m2[] = { 0x83, 0xec, 0x28, 0x53, 0x55 };   /* FUN_1074633c */
            static const unsigned char m3[] = { 0x83, 0xec, 0x14, 0x53, 0x55 };   /* FUN_107711fb */
            midhook(0x1355b, 5, hMerge, m1);
            midhook(0x4633c, 5, hTryPair, m2);
            midhook(0x711fb, 5, hTryMove, m3);
            {
                static const unsigned char m4[] = { 0x8b, 0x44, 0x24, 0x20, 0x85, 0xc0 };   /* 107464fb */
                static const unsigned char m5[] = { 0x8b, 0x4c, 0x24, 0x10, 0x8b, 0x7c, 0x24, 0x18 };   /* 10746586 */
                midhook(0x464fb, 6, hPairCount, m4);
                midhook(0x46586, 8, hPairSize, m5);
            }
            if (getenv("C2DAG_IL")) {
                static const unsigned char i1[] = { 0xe8, 0x64, 0x27, 0xff, 0xff };   /* 1070ea0f call */
                static const unsigned char i2[] = { 0xba, 0x01, 0x00, 0x00, 0x00 };   /* 1070ec8a mov edx,1 */
                static const unsigned char i3[] = { 0xe8, 0xd4, 0x24, 0xff, 0xff };   /* 1070ec9f call */
                midhook(0xea0f, 5, hIlPre, i1);
                midhook(0xec8a, 5, hIlMerge, i2);
                midhook(0xec9f, 5, hIlPost, i3);
            }
        }
    }
}

__declspec(dllexport) int __stdcall InvokeCompilerPass(void *a, void *b, void *c)
{
    if (!real)
        install();
    return realInvoke(a, b, c);
}

__declspec(dllexport) int __stdcall AbortCompilerPass(void *a)
{
    if (!real)
        install();
    return realAbort(a);
}
