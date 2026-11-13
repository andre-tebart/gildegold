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
static void *savedP, *savedRet;

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

static void __cdecl dump(void)
{
    void *first, *last, *n, *e, *t;
    void *nodes[4096];
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

/* second hook: the allocator's priority pass (FUN_10707a0b), first return per function */
static unsigned char *tramp2;
static void *savedRet2;
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
        pop eax
        mov savedRet2, eax
        call dword ptr [tramp2]
        pushad
        call dump2
        popad
        jmp dword ptr [savedRet2]
    }
}

static __declspec(naked) void stub(void)
{
    __asm {
        mov savedP, ecx
        pop eax
        mov savedRet, eax
        call dword ptr [tramp]
        pushad
        call dump
        popad
        jmp dword ptr [savedRet]
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
            midhook(0x7b6e, 5, hP, e4);
            midhook(0x12437, 6, hXJ, e5);
            midhook(0x7b31, 5, hBlock, e1);
            midhook(0x7c42, 5, hUse, e2);
            midhook(0x7ed2, 7, hDef, e3);
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
