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
