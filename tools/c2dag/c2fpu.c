/* c2.dll proxy for VC6 SP5+PP (C2 13.00.9044) for the x87 decisions of one function (C2FPU_FUNC, plain name),
   written to the file C2FPU_OUT:
     @@ FUNC STAGE   the IL before each pass of the code generator's driver FUN_1076e010, inside the allocation
                     wrapper FUN_1072e2f0, before the stackifier FUN_10731359 and after the last pass (`final`):
                     one line per tuple `op class flags line | uses | defs ; mnemonic @tuple`; an operand is a
                     register (st0..st7, eax..), a candidate (%name: no FPU slot, $name: the slot), m:symbol
                     (memory) or kN (another kind), then /type.flag.flag (type 4004 float, 4008 double)
     FPQ / Q ..      the FP allocator's queue (FUN_1076697d -> FUN_10767401) after each priority pass
     OK / NO ..      each stack fit (FUN_10767ca5) of a candidate: name, prio, savings, tie, kind, size, the IL
                     lines of its definitions and deaths
   C2FPU_RAW=1 adds the raw words of each operand's register / candidate record.
   Build: cl /nologo /LD /O1 c2fpu.c /Fec2.dll /link /BASE:0x20000000 user32.lib (c2fpu.py does it). */
#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef int (__stdcall *INVOKE)(void *, void *, void *);
typedef int (__stdcall *ABORT)(void *);

static HMODULE real;
static INVOKE realInvoke;
static ABORT realAbort;
static FILE *out;

#define U32(p, o) (*(unsigned long *)((char *)(p) + (o)))
#define U16(p, o) (*(unsigned short *)((char *)(p) + (o)))
#define U8(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define PTR(p, o) (*(void **)((char *)(p) + (o)))

static const char *curName(void)
{
    char *cur = *(char **)((char *)real + 0xad730), *nm = 0;
    if (!IsBadReadPtr(cur, 0x1c))
        nm = *(char **)(cur + 0x18);
    return nm && !IsBadStringPtrA(nm, 128) ? nm : "?";
}

static int wanted(void)
{
    const char *w = getenv("C2FPU_FUNC"), *nm = curName();
    size_t n;
    if (!w || !*w)
        return 0;
    if (*nm == '_' || *nm == '@')
        nm++;
    n = strlen(w);
    return !strncmp(nm, w, n) && (nm[n] == 0 || nm[n] == '@');
}

/* the back end's register table (RVA 0xc49e8, 0x58 bytes per register): 2 eax .. 9 edi, 32 st0 .. 39 st7 */
static void symname(void *sym, int kind)
{
    static const char *regs[] = { "r0", "r1", "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
    unsigned long d = (unsigned long)sym - ((unsigned long)real + 0xc49e8);
    char *nm;
    const char *m = kind == 2 ? "m:" : "";
    if (d < 0x58 * 80 && d % 0x58 == 0) {
        d /= 0x58;
        if (d < 10)
            fprintf(out, "%s%s", m, regs[d]);
        else if (d >= 32 && d < 40)
            fprintf(out, "%sst%lu", m, d - 32);
        else
            fprintf(out, "%sr%lu", m, d);
        return;
    }
    nm = PTR(sym, 0x18);
    if (nm && !IsBadStringPtrA(nm, 96) && nm[0] > 32 && nm[0] < 127) {
        fprintf(out, "%s%s", m, nm);
        return;
    }
    nm = !IsBadReadPtr(PTR(sym, 0), 0x20) ? PTR(PTR(sym, 0), 0x18) : 0;
    if (nm && !IsBadStringPtrA(nm, 96) && nm[0] > 32 && nm[0] < 127)
        fprintf(out, "%s%s", m, nm);
    else
        fprintf(out, "%s%s#%lx", m, U32(sym, 4) == 0x100d ? "const" : U32(sym, 4) == 3 ? "temp" : "s", U32(sym, 0x1c));
}

/* an allocation candidate (an operand's +0x18 object whose byte +4 is 2): [0] its symbol (+4 kind: 3 temp, 0xd
   constant; +0x20 size), whose [0] holds the source name at +0x18 */
static const char *candName(void *c)
{
    static char b[4][128];
    static int k;
    char *o = b[k++ & 3], *nm = 0;
    void *c0 = PTR(c, 0), *s;
    if (!c0 || IsBadReadPtr(c0, 0x24)) {
        sprintf(o, "?%lx", (unsigned long)c & 0xffff);
        return o;
    }
    s = PTR(c0, 0);
    if (s && !IsBadReadPtr(s, 0x1c))
        nm = PTR(s, 0x18);
    if (U8(c0, 4) == 4 && !IsBadReadPtr(PTR(c0, 8), 0x20)) {   /* a field: [2] the aggregate, +0x24 the offset */
        void *a = PTR(c0, 8);
        nm = PTR(a, 0x18);
        if (!nm || IsBadStringPtrA(nm, 96) || nm[0] <= 32 || nm[0] >= 127)
            nm = !IsBadReadPtr(PTR(a, 0), 0x20) ? PTR(PTR(a, 0), 0x18) : 0;
        if (nm && !IsBadStringPtrA(nm, 96) && nm[0] > 32 && nm[0] < 127) {
            sprintf(o, "%.80s+%lx@%lx", nm, U32(c0, 0x24), (unsigned long)c & 0xffff);
            return o;
        }
    }
    if (nm && !IsBadStringPtrA(nm, 96) && nm[0] > 32 && nm[0] < 127)
        sprintf(o, "%.80s@%lx", nm, (unsigned long)c & 0xffff);
    else
        sprintf(o, "%s#%lx@%lx", U8(c0, 4) == 0xd ? "const" : U8(c0, 4) == 3 ? "temp" : "k", U32(c0, 0x1c),
                (unsigned long)c & 0xffff);
    return o;
}

/* an operand list: kind 1 register / symbol (+0x18 -> +8 symbol), 2 memory (+0x14 -> +8 base symbol), 7 immediate;
   type at +0xa (0x4004 float, 0x4008 double, 0x400a long double, 0x10xx int), flag bytes +0x10 / +0x11 */
static void opnds(void *o)
{
    int g = 0;
    for (; o && g < 64 && !IsBadReadPtr(o, 0x14); o = PTR(o, 0), g++) {
        unsigned kind = U8(o, 8);
        void *ref = kind == 1 ? PTR(o, 0x18) : kind == 2 ? PTR(o, 0x14) : 0, *sym = 0;
        fprintf(out, " ");
        if (ref && !IsBadReadPtr(ref, 12))
            sym = PTR(ref, 8);
        if (kind == 1 && ref && !IsBadReadPtr(ref, 0x48) && U8(ref, 4) == 2)
            fprintf(out, "%c%s", PTR(ref, 0x10) == (char *)real + 0xc54e8 ? '$' : '%', candName(ref));
        else if (sym && !IsBadReadPtr(sym, 0x20))
            symname(sym, kind);
        else
            fprintf(out, "k%x", kind);
        if (kind == 1 && ref && !IsBadReadPtr(ref, 0x20) && U32(ref, 0x1c) >= 0x1f && U32(ref, 0x1c) < 0x27)
            fprintf(out, "=st%lu", U32(ref, 0x1c) - 0x1f);
        if (getenv("C2FPU_RAW") && ref && !IsBadReadPtr(ref, 0x30)) {
            int q;
            fprintf(out, "{");
            for (q = 0; q < 0x30; q += 4)
                fprintf(out, "%lx ", U32(ref, q));
            fprintf(out, "}");
        }
        fprintf(out, "/%x.%x.%x", U16(o, 10), U8(o, 0x10), U8(o, 0x11));
    }
}

static void dumpIl(void *fn, const char *stage)
{
    void *x, *t, *end;
    int g = 0;
    if (!out || !wanted() || IsBadReadPtr(fn, 0x10))
        return;
    x = PTR(fn, 8);
    if (IsBadReadPtr(x, 8) || IsBadReadPtr(PTR(x, 0), 0x20) || IsBadReadPtr(PTR(x, 4), 0x24))
        return;
    t = PTR(PTR(x, 0), 0x1c);
    end = PTR(PTR(x, 4), 0x20);
    if (IsBadReadPtr(t, 4))
        return;
    fprintf(out, "@@ %s %s\n", curName(), stage);
    for (t = PTR(t, 0); t && t != end && g < 40000 && !IsBadReadPtr(t, 0x28); t = PTR(t, 0), g++) {
        unsigned cls = U8(t, 8);
        if (cls == 0x1a)
            fprintf(out, " L%04lx:\n", (unsigned long)t & 0xffff);
        else {
            fprintf(out, "   %lx %x %x %d |", U32(t, 4), cls, U8(t, 9), (int)*(short *)((char *)t + 0x10));
            opnds(PTR(t, 0x1c));
            fprintf(out, " |");
            opnds(PTR(t, 0x20));
            {   /* the mnemonic: the back end's name table (RVA 0xbc7ec) holds opcode N at index N + 1 */
                unsigned long op = U32(t, 4);
                char *nm = op < 0x300 ? *(char **)((char *)real + 0xbc7ec + (op + 1) * 4) : 0;
                if (nm && !IsBadStringPtrA(nm, 32) && nm[0] > 32 && nm[0] < 127)
                    fprintf(out, " ; %.20s @%lx", nm, (unsigned long)t);
            }
            fprintf(out, "\n");
        }
    }
    fflush(out);
}

/* the FP class allocator FUN_1076697d (class 1 = DAT_107ad974): the queue after each priority pass (FUN_10766d7b,
   sorted by FUN_10767323: prio +0xc descending, tie +0x44) as handed to FUN_10767401 (which drops every candidate
   with savings +0x40 <= 0), then each FUN_10767ca5 stack fit: OK (register st0 = an FPU stack slot) or NO */
static void points(const char *tag, void *l)
{
    int g = 0;
    fprintf(out, " %s", tag);
    for (; l && g < 64 && !IsBadReadPtr(l, 8); l = PTR(l, 0), g++) {
        void *t = PTR(l, 4);
        if (!t || IsBadReadPtr(t, 0x14))
            fprintf(out, " ?");
        else if (U8(t, 8) == 0x1a)
            fprintf(out, " L%04lx", (unsigned long)t & 0xffff);
        else
            fprintf(out, " %d:%lx", (int)*(short *)((char *)t + 0x10), U32(t, 4));
    }
}
static void cand(const char *tag, void *c)
{
    void *c0 = PTR(c, 0);
    fprintf(out, "%s %s prio %ld sav %ld tie %lx kind %x size %ld", tag, candName(c), (long)U32(c, 0xc),
            (long)U32(c, 0x40), U32(c, 0x44), c0 && !IsBadReadPtr(c0, 0x24) ? U8(c0, 4) : 0xff,
            c0 && !IsBadReadPtr(c0, 0x24) ? (long)U32(c0, 0x20) : -1L);
    points("defs", PTR(c, 0x38));
    points("deaths", PTR(c, 0x3c));
    fprintf(out, "\n");
}
static void __cdecl hQueue(unsigned long *r)
{
    void *c = (void *)r[6];
    int g = 0;
    if (!out || !wanted())
        return;
    fprintf(out, "FPQ %s\n", curName());
    for (; c && g < 2000 && !IsBadReadPtr(c, 0x48); c = PTR(c, 0x14), g++)
        cand("Q", c);
    fflush(out);
}
static void __cdecl hFitOk(unsigned long *r) { if (out && wanted()) cand("OK", (void *)r[3]); }
static void __cdecl hFitNo(unsigned long *r) { if (out && wanted()) cand("NO", (void *)r[3]); }
static void __cdecl hStackify(unsigned long *r) { dumpIl((void *)r[1], "before the stackifier 10731359"); }

static const char *stageName[16];
static void __cdecl hStage(unsigned long *r, int k) { dumpIl((void *)r[2], stageName[k]); }
#define H(k) static void __cdecl h##k(unsigned long *r) { hStage(r, k); }
H(0) H(1) H(2) H(3) H(4) H(5) H(6) H(7) H(8) H(9) H(10) H(11) H(12) H(13) H(14) H(15)
static void (__cdecl *hs[16])(unsigned long *) = { h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15 };

/* a hook at a call rel32: pushad, pushfd, handler(saved regs: r[0] eflags, r[1] edi, r[2] esi, ..), then the call */
static void midhook(unsigned long rva, void (__cdecl *h)(unsigned long *))
{
    unsigned char *fn = (unsigned char *)real + rva, *p, *s;
    DWORD old;
    if (fn[0] != 0xe8)
        return;
    s = p = (unsigned char *)VirtualAlloc(0, 64, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    *p++ = 0x60; *p++ = 0x9c; *p++ = 0x54;
    *p++ = 0xe8; *(long *)p = (long)h - (long)(p + 4); p += 4;
    *p++ = 0x83; *p++ = 0xc4; *p++ = 0x04;
    *p++ = 0x9d; *p++ = 0x61;
    *p++ = 0xe8; *(long *)p = (long)(fn + 5 + *(long *)(fn + 1)) - (long)(p + 4); p += 4;
    *p++ = 0xe9; *(long *)p = (long)(fn + 5) - (long)(p + 4);
    VirtualProtect(fn, 5, PAGE_EXECUTE_READWRITE, &old);
    fn[0] = 0xe9; *(long *)(fn + 1) = (long)s - (long)(fn + 5);
    VirtualProtect(fn, 5, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 5);
}

/* a hook over len bytes of position-independent instructions */
static void midhookN(unsigned long rva, int len, void (__cdecl *h)(unsigned long *), const unsigned char *expect)
{
    unsigned char *fn = (unsigned char *)real + rva, *p, *s;
    DWORD old;
    if (memcmp(fn, expect, len))
        return;
    s = p = (unsigned char *)VirtualAlloc(0, 64, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    *p++ = 0x60; *p++ = 0x9c; *p++ = 0x54;
    *p++ = 0xe8; *(long *)p = (long)h - (long)(p + 4); p += 4;
    *p++ = 0x83; *p++ = 0xc4; *p++ = 0x04;
    *p++ = 0x9d; *p++ = 0x61;
    memcpy(p, fn, len); p += len;
    *p++ = 0xe9; *(long *)p = (long)(fn + len) - (long)(p + 4);
    VirtualProtect(fn, len, PAGE_EXECUTE_READWRITE, &old);
    fn[0] = 0xe9; *(long *)(fn + 1) = (long)s - (long)(fn + 5);
    memset(fn + 5, 0x90, len - 5);
    VirtualProtect(fn, len, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, len);
}

/* the driver's call sites (esi = the function at each): the stage is named after the pass it precedes */
static const struct { unsigned long rva; const char *name; } sites[] = {
    { 0x6e0e2, "before 1071cd72" }, { 0x6e0f1, "before 1073e0ee" }, { 0x6e0f8, "before 10728f2e" },
    { 0x6e10b, "before 1072c9a2" }, { 0x6e112, "before 1072e2f0 (allocation)" },
    { 0x2e310, "alloc: before 1071be2a" }, { 0x2e323, "alloc: before 10730fc2" }, { 0x2e336, "alloc: before 1073122d" }, { 0x6e119, "before 1070cdb9" },
    { 0x6e120, "before 1070ea02" }, { 0x6e127, "before 1070eb58" }, { 0x6e136, "before 1070ec03" },
    { 0x6e13d, "before 1070ec81" }, { 0x6e151, "before 10713cbe" }, { 0x6e158, "before 10716e01" },
    { 0x6e15d, "final" },
};

static void install(void)
{
    char path[MAX_PATH], *s;
    const char *o;
    int k;
    GetModuleFileNameA(GetModuleHandleA("c2.dll"), path, MAX_PATH);
    s = strrchr(path, '\\');
    strcpy(s ? s + 1 : path, "c2real.dll");
    real = LoadLibraryA(path);
    if (!real) {
        MessageBoxA(0, path, "c2fpu: cannot load", 0);
        ExitProcess(3);
    }
    realInvoke = (INVOKE)GetProcAddress(real, "_InvokeCompilerPass@12");
    realAbort = (ABORT)GetProcAddress(real, "_AbortCompilerPass@4");
    o = getenv("C2FPU_OUT");
    if (o && *o)
        out = fopen(o, "a");
    for (k = 0; k < sizeof sites / sizeof sites[0]; k++) {
        stageName[k] = sites[k].name;
        midhook(sites[k].rva, hs[k]);
    }
    {
        static const unsigned char ok[] = { 0x8b, 0x55, 0x1c, 0x8b, 0xcb };               /* 10766a88 */
        static const unsigned char no[] = { 0x85, 0xf6, 0x8b, 0x44, 0x24, 0x18 };         /* 10766ad2 */
        midhook(0x669dd, hQueue);
        midhook(0x66a31, hQueue);
        midhookN(0x66a88, 5, hFitOk, ok);
        midhookN(0x66ad2, 6, hFitNo, no);
        midhook(0x66a5f, hStackify);
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
