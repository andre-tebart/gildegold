/* c2.dll proxy (C2 13.00.9044): dumps the stack-frame layout decision of stack.c.
   Output file: env C2FRM_OUT. Lines:
   R name            one operand reference counted (FUN_10713312), in walk order (only if C2FRM_REFS set)
   L idx name size count kind   the symbol list at entry of the packer FUN_1074764f (ascending size, then count)
   B/A i size weight dens align : members    slot table before / after the density sort */
#include <windows.h>
#include <stdio.h>
typedef int (__stdcall *INVOKE)(void *, void *, void *);
typedef int (__stdcall *ABORT)(void *);
static HMODULE real; static INVOKE realInvoke; static ABORT realAbort; static FILE *out; static int refs;
#define U32(p, o) (*(unsigned long *)((char *)(p) + (o)))
#define U8(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define PTR(p, o) (*(void **)((char *)(p) + (o)))
#define G(rva) (*(unsigned long *)((char *)real + (rva)))
static const char *sname(void *sym)
{
    static char b[64]; void *fe; char *nm;
    if (!sym || IsBadReadPtr(sym, 0x58)) return "?";
    fe = PTR(sym, 0);
    if (fe && !IsBadReadPtr(fe, 0x20)) {
        nm = PTR(fe, 0x18);
        if (nm && !IsBadStringPtrA(nm, 96) && nm[0] > 32 && nm[0] < 127) return nm;
        sprintf(b, "#%lx", U32(fe, 0x1c)); return b;
    }
    sprintf(b, "t%lx", (unsigned long)sym & 0xffff); return b;
}
static void __cdecl hRef(unsigned long *r)
{
    if (out && refs) fprintf(out, "R %s +%ld\n", sname((void *)r[7]), r[6]);
}
static void __cdecl hEntry(unsigned long *r)
{
    void *s; int i = 0;
    if (!out) return;
    { void *fs = PTR((void *)r[7], 0); char *fn = (fs && !IsBadReadPtr(fs, 0x20)) ? (char *)PTR(fs, 0x18) : 0;
      fprintf(out, "FUNC %s nsym %lu nfixed %lu\n", fn && !IsBadStringPtrA(fn, 200) ? fn : "?", G(0xb15f8), G(0xb15f4)); }
    for (s = (void *)G(0xb15fc); s; s = PTR(s, 0x2c), i++)
        fprintf(out, "L %d %s size %lu count %lu kind %u idx %ld\n", i, sname(s), U32(s, 0x20), U32(s, 0x34), U8(s, 4), U32(s, 0x38));
    fflush(out);
}
static void slots(const char *tag)
{
    unsigned long n = G(0xb15e4), i; unsigned long *t = (unsigned long *)G(0xb15e8); void **idx = (void **)G(0xb15f0);
    if (!out) return;
    for (i = 0; i < n; i++, t += 6) {
        unsigned long *node;
        fprintf(out, "%s %lu size %lu weight %lu dens %lu align %lu :", tag, i, t[2], t[3], t[2] ? t[3] * 1000 / t[2] : 0, t[5]);
        for (node = *(unsigned long **)t[0]; node; node = (unsigned long *)node[1]) {
            int b;
            for (b = 0; b < 32; b++) if (node[2] >> b & 1) fprintf(out, " %s", sname(idx[node[0] + b]));
        }
        fprintf(out, "\n");
    }
    fflush(out);
}
static void __cdecl hBefore(unsigned long *r) { if (out) fprintf(out, "total %lu\n", U32(r[4], 0x10)); slots("B"); }
static void __cdecl hAfter(unsigned long *r) { slots("A"); }

static int tups;
static void oplist(const char *tag, void *o)
{
    int g = 0;
    for (; o && g < 64; o = PTR(o, 0), g++) {
        unsigned k = U8(o, 8); void *ref = 0, *sym = 0;
        if (k == 2 || k == 3) ref = PTR(o, 0x14);
        else if (k == 5 || k == 6) ref = PTR(o, 0x28);
        if (ref && !IsBadReadPtr(ref, 12)) sym = PTR(ref, 8);
        if (sym && !IsBadReadPtr(sym, 0x58) && (U8(sym, 4) == 3 || U8(sym, 4) == 4 || U8(sym, 4) == 5))
            fprintf(out, " %s%u:%s", tag, k, sname(sym));
        else
            fprintf(out, " %s%u", tag, k);
    }
}
static void __cdecl hWalk(unsigned long *r)
{
    void *blk, *t; int bn = 0;
    if (!out || !tups) return;
    fprintf(out, "WALK\n");
    for (blk = PTR(PTR((void *)r[7], 8), 0); blk; blk = PTR(blk, 0), bn++) {
        fprintf(out, "T block %d\n", bn);
        for (t = PTR(PTR(blk, 0x1c), 0); t && t != PTR(blk, 0x20); t = PTR(t, 0)) {
            if (!(U8(t, 9) & 1)) continue;
            fprintf(out, "T  op %lx k%u", U32(t, 4), U8(t, 8));
            oplist("u", PTR(t, 0x1c)); oplist("d", PTR(t, 0x20));
            fprintf(out, "\n");
        }
    }
    fflush(out);
}
static void midhook(unsigned long rva, int len, void (__cdecl *h)(unsigned long *), const unsigned char *expect)
{
    unsigned char *fn = (unsigned char *)real + rva, *p, *s; DWORD old;
    if (memcmp(fn, expect, len == 6 ? 2 : len)) { if (out) fprintf(out, "hook %lx mismatch\n", rva); return; }
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
static void install(void)
{
    char path[MAX_PATH], *s; const char *o;
    static const unsigned char e1[] = { 0x55, 0x56, 0x57, 0x8b, 0xfa };
    static const unsigned char e2[] = { 0x83, 0xec, 0x1c, 0x53, 0x55 };
    static const unsigned char e3[] = { 0x81, 0x7c, 0x24, 0x10, 0x80, 0x00, 0x00, 0x00 };
    static const unsigned char e4[] = { 0x8b, 0x15 };
    GetModuleFileNameA(GetModuleHandleA("c2.dll"), path, MAX_PATH);
    s = strrchr(path, (char)92); strcpy(s ? s + 1 : path, "c2real.dll");
    real = LoadLibraryA(path);
    if (!real) ExitProcess(3);
    realInvoke = (INVOKE)GetProcAddress(real, "_InvokeCompilerPass@12");
    realAbort = (ABORT)GetProcAddress(real, "_AbortCompilerPass@4");
    o = getenv("C2FRM_OUT");
    if (o && *o) out = fopen(o, "w");
    refs = getenv("C2FRM_REFS") != 0;
    { static const unsigned char e5[] = { 0x8b, 0x41 }; tups = getenv("C2FRM_TUP") != 0; midhook(0xddc7, 6, hWalk, e5); }
    midhook(0x13312, 5, hRef, e1);
    midhook(0x4764f, 5, hEntry, e2);
    midhook(0x47705, 8, hBefore, e3);
    midhook(0x47731, 6, hAfter, e4);
}
__declspec(dllexport) int __stdcall InvokeCompilerPass(void *a, void *b, void *c) { if (!real) install(); return realInvoke(a, b, c); }
__declspec(dllexport) int __stdcall AbortCompilerPass(void *a) { if (!real) install(); return realAbort(a); }
