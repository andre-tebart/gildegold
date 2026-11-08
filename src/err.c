/* err.c -- original file name unknown ("Error-Handler", "<exe dir>\<name>_error.log").
   Error handler and log channels, with an unhandled-exception filter that writes a crash dump with a
   symbolized stack walk. Extent in GildeGold.exe (inferred): 0x4214a0..0x4223c0.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/err.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "err.h"

/* symbol map: header, then nsyms ERRSYM records (each 0x10 + len bytes) at data + symoff, and
   nstrings ERRSTR records (each 4 + len bytes) at data */
typedef struct ERRMAP {
    int unk0;
    unsigned int nstrings;          /* 0x04 */
    unsigned int nsyms;             /* 0x08 */
    int symoff;                     /* 0x0c */
} ERRMAP;

typedef struct ERRSYM {
    int len;                        /* length of name including the 0 */
    unsigned int lo;                /* 0x04 */
    unsigned int hi;                /* 0x08 */
    unsigned int file;              /* 0x0c, string index */
    char name[1];                   /* 0x10 */
} ERRSYM;

typedef struct ERRSTR {
    int len;
    char s[1];
} ERRSTR;

typedef struct ERRWALK {
    ERRMAP *map;
    HANDLE file;                    /* 0x04 */
    unsigned int addr;              /* 0x08 */
    unsigned int prev;              /* 0x0c */
    unsigned int last;              /* 0x10 */
    unsigned int count;             /* 0x14 */
    unsigned int repeat;            /* 0x18 */
} ERRWALK;

int err_Channels[12] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800 };  /* 0x5f6d48 */
/* 0x5f6d78 */
char err_Names[12][32] = {
    "_error.log", "_ai.log", "_sim.log", "_dbg.log", "_script.log", "_cutscene.log",
    "_meister.log", "_d3.log", "_stats.log", "_msx.log", "___tracker.log", "_meister2.log"
};

int err_Started = 0;  /* 0x632368 */
void (*err_CrashCB)(HANDLE) = 0;  /* 0x63236c, set by err_Init, called by the crash filter with the log handle (or -1 when no log was written) */
unsigned int err_Flags = 0;  /* 0x632370 */
char err_ShowUnknown = 0;  /* 0x632374 */
char err_Buf[8192] = {0};  /* 0x1461440 */
HWND err_Hwnd = 0;  /* 0x1463440 */
char err_Dir[260] = {0};  /* 0x1463460 */
HANDLE err_Console = 0;  /* 0x1463564 */

LONG WINAPI err_ExceptionFilter(EXCEPTION_POINTERS *ep);
ERRSTR *err_GetString(ERRMAP *map, unsigned int i);
char err_WriteSymbol(ERRWALK *w);
char err_IsCodeProtect(unsigned char prot);
char err_CheckCall(char *p, int n, int base, int *target);

/* 0x4214a0, writes a line to the channel's log file, the debugger and the console as the ERR_ flags say */
void err_Write(char *text, unsigned int flags, int channel)
{
    char path[260];
    DWORD n;
    FILE *f;
    int i;

    if (flags & ERR_LOG) {
        for (i = 0; i < 12; i++) {
            if (channel == err_Channels[i]) {
                sprintf(path, "%s\\%s", err_Dir, err_Names[i]);
                f = fopen(path, "at");
                if (f)
                    goto write;
                break;
            }
        }
        sprintf(path, "%s\\%s", err_Dir, "_error.log");
        f = fopen(path, "at");
        if (f) {
write:
            fputs(text, f);
            fputs("\n", f);
            fclose(f);
        }
    }
    if (flags & ERR_DEBUG) {
        OutputDebugString(text);
        OutputDebugString("\n");
    }
    if (flags & ERR_CONSOLE) {
        WriteConsole(err_Console, text, strlen(text), &n, NULL);
        WriteConsole(err_Console, "\n", 1, &n, NULL);
    }
}
/* 0x4215c0, unhandled exception filter: writes <exe>.dbg-symbolized crash info to _crashdump.log */

LONG WINAPI err_ExceptionFilter(EXCEPTION_POINTERS *ep)
{
    char handled = 0;
    CONTEXT *ctx = ep->ContextRecord;
    char path[256];
    char text[256];
    MEMORY_BASIC_INFORMATION mbi;
    ERRWALK w;
    HANDLE mapfile, log;
    ERRMAP *map;
    unsigned int got, size, key, *p, *stack;
    int remaining, len;
    DWORD n;
    char *dot;

    SetUnhandledExceptionFilter(NULL);
    if (GetModuleFileName(NULL, path, sizeof(path))) {
        dot = path;
        while (*dot)
            dot++;
        while (dot >= path && *dot != '.')
            dot--;
        if (*dot == '.') {
            *++dot = 'd';
            *++dot = 'b';
            *++dot = 'g';
            dot[1] = 0;
        }
        mapfile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (mapfile != INVALID_HANDLE_VALUE) {
            ReadFile(mapfile, &size, 4, &got, NULL);
            if (got == 4) {
                map = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
                if (map) {
                    ReadFile(mapfile, map, size, &got, NULL);
                    if (size == got) {
                        log = CreateFile("_crashdump.log", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                        key = map->unk0;
                        p = (unsigned int *)(map + 1);
                        for (size = 0; size < (got >> 2) - 4; size++) {
                            key = (key << 15) | (key >> 17);
                            *p++ ^= key;
                        }
                        if (log != INVALID_HANDLE_VALUE) {
                            HANDLE proc = GetCurrentProcess();
                            char crlf[] = "\r\n";
                            char sp = ' ';
                            char sep[] = "------------------------\r\n";
                            char win[7];
                            remaining = 0x800;
                            stack = (unsigned int *)ctx->Esp;
                            SetFilePointer(log, -1, NULL, FILE_END);
                            WriteFile(log, sep, 0x1a, &n, NULL);
                            len = GetDateFormat(0x400, 2, NULL, NULL, text, 0x100);
                            if (len) {
                                WriteFile(log, text, len - 1, &n, NULL);
                                WriteFile(log, &sp, 1, &n, NULL);
                            }
                            len = GetTimeFormat(0x400, 8, NULL, NULL, text, 0x100);
                            if (len) {
                                WriteFile(log, text, len - 1, &n, NULL);
                                WriteFile(log, crlf, 2, &n, NULL);
                            }
                            switch (ep->ExceptionRecord->ExceptionCode) {
#define ERR_EXC(code, name) case code: WriteFile(log, name, sizeof(name) - 1, &n, NULL); break
                            ERR_EXC(EXCEPTION_BREAKPOINT, "BREAKPOINT");
                            ERR_EXC(EXCEPTION_SINGLE_STEP, "SINGLE_STEP");
                            case EXCEPTION_ACCESS_VIOLATION:
                                WriteFile(log, "ACCESS_VIOLATION ", 0x11, &n, NULL);
                                if (ep->ExceptionRecord->ExceptionInformation[0] == 0)
                                    WriteFile(log, "READ ", 5, &n, NULL);
                                else
                                    WriteFile(log, "WRITE ", 6, &n, NULL);
                                sprintf(text, "0x%08X", ep->ExceptionRecord->ExceptionInformation[1]);
                                WriteFile(log, text, strlen(text), &n, NULL);
                                break;
                            ERR_EXC(EXCEPTION_IN_PAGE_ERROR, "IN_PAGE_ERROR");
                            ERR_EXC(EXCEPTION_ILLEGAL_INSTRUCTION, "ILLEGAL_INSTRUCTION");
                            ERR_EXC(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "ARRAY_BOUNDS_EXCEEDED");
                            ERR_EXC(EXCEPTION_FLT_DENORMAL_OPERAND, "FLT_DENORMAL_OPERAND");
                            ERR_EXC(EXCEPTION_INVALID_DISPOSITION, "INVALID_DISPOSITION");
                            ERR_EXC(EXCEPTION_NONCONTINUABLE_EXCEPTION, "NONCONTINUABLE_EXCEPTION");
                            ERR_EXC(EXCEPTION_FLT_DIVIDE_BY_ZERO, "FLT_DIVIDE_BY_ZERO");
                            ERR_EXC(EXCEPTION_FLT_INEXACT_RESULT, "FLT_INEXACT_RESULT");
                            ERR_EXC(EXCEPTION_FLT_INVALID_OPERATION, "FLT_INVALID_OPERATION");
                            ERR_EXC(EXCEPTION_FLT_OVERFLOW, "FLT_OVERFLOW");
                            ERR_EXC(EXCEPTION_FLT_STACK_CHECK, "FLT_STACK_CHECK");
                            ERR_EXC(EXCEPTION_FLT_UNDERFLOW, "FLT_UNDERFLOW");
                            ERR_EXC(EXCEPTION_INT_DIVIDE_BY_ZERO, "INT_DIVIDE_BY_ZERO");
                            ERR_EXC(EXCEPTION_INT_OVERFLOW, "INT_OVERFLOW");
                            ERR_EXC(EXCEPTION_PRIV_INSTRUCTION, "PRIV_INSTRUCTION");
                            ERR_EXC(EXCEPTION_STACK_OVERFLOW, "STACK_OVERFLOW");
#undef ERR_EXC
                            default:
                                goto nocode;
                            }
                            WriteFile(log, crlf, 2, &n, NULL);
nocode:
                            w.map = map;
                            w.file = log;
                            w.addr = ctx->Eip;
                            w.prev = 0;
                            w.last = 0;
                            w.count = 0;
                            w.repeat = 0;
                            stack--;
                            do {
                                VirtualQuery((void *)w.addr, &mbi, sizeof(mbi));
                                if (err_IsCodeProtect((unsigned char)mbi.Protect) && mbi.State == MEM_COMMIT) {
                                    if (w.addr != ctx->Eip) {
                                        for (len = 7; len > 0; len--)
                                            if (ReadProcessMemory(proc, (void *)(w.addr - len), win + 7 - len, len, NULL))
                                                break;
                                        if (err_CheckCall(win + 7, len, w.addr, (int *)&w.prev) <= 0)
                                            goto next;
                                    }
                                    if (err_WriteSymbol(&w))
                                        remaining--;
                                }
next:
                                stack++;
                            } while (remaining > 0 && ReadProcessMemory(proc, stack, &w.addr, 4, NULL));
                            if (err_CrashCB) {
                                err_CrashCB(log);
                                handled = 1;
                            }
                            WriteFile(log, &crlf[2], 1, &n, NULL);
                            CloseHandle(log);
                        }
                    }
                    VirtualFree(map, 0, MEM_RELEASE);
                }
            }
            CloseHandle(mapfile);
        }
    }
    if (err_CrashCB && !handled)
        err_CrashCB((HANDLE)-1);
    UnhandledExceptionFilter(ep);
    return EXCEPTION_EXECUTE_HANDLER;
}
/* 0x421d50, writes the symbol (and its file) that contains w->addr to the crash log; 0 when nothing
   matched or the walk is over (w->last / w->repeat / w->count fold repeated frames) */

char err_WriteSymbol(ERRWALK *w)
{
    ERRMAP *map = w->map;
    ERRSYM *sym = (ERRSYM *)((char *)map + map->symoff + 0x10);
    unsigned int i;
    DWORD n;

    for (i = 0; i < map->nsyms; i++) {
        if (w->addr >= sym->lo && w->addr <= sym->hi) {
            char unknown[] = "??? ";
            char arrow[] = "--> ";
            char *prefix = NULL;
            char show = err_ShowUnknown;

            if (w->last == 0)
                w->last = sym->lo;
            else if (w->last == w->prev) {
                w->last = sym->lo;
                w->repeat++;
            } else {
                prefix = unknown;
                if (w->count > 0 && w->repeat <= 1) {
                    prefix = arrow;
                    w->last = sym->lo;
                    w->count--;
                } else if (!show)
                    return 0;
            }
            {
                ERRSTR *file = err_GetString(map, sym->file);
                char crlf[] = "\r\n";
                char sp[] = " ";

                if (prefix)
                    WriteFile(w->file, prefix, 4, &n, NULL);
                WriteFile(w->file, sym->name, sym->len - 1, &n, NULL);
                if (file) {
                    WriteFile(w->file, sp, 1, &n, NULL);
                    WriteFile(w->file, file->s, file->len - 1, &n, NULL);
                }
                WriteFile(w->file, crlf, 2, &n, NULL);
            }
            return 1;
        }
        sym = (ERRSYM *)((char *)sym + sym->len + 0x10);
    }
    return 0;
}
/* 0x421ed0, the i-th string record of the symbol map (walking the length-prefixed records), NULL past
   the end */

ERRSTR *err_GetString(ERRMAP *map, unsigned int i)
{
    ERRSTR *s;

    if (i < map->nstrings) {
        s = (ERRSTR *)(map + 1);
        for (; i > 0; i--)
            s = (ERRSTR *)((char *)s + s->len + 4);
        return s;
    }
    return NULL;
}
/* 0x421f00, whether prot marks memory the stack walk may scan as code: executable pages always; a
   read-only or read-write page only while the exe's own code page is read-only or read-write too */

char err_IsCodeProtect(unsigned char prot)
{
    MEMORY_BASIC_INFORMATION mbi;

    VirtualQuery(err_IsCodeProtect, &mbi, sizeof(mbi));
    switch (prot) {
    case PAGE_READONLY:
    case PAGE_READWRITE:
        return mbi.Protect == PAGE_READONLY || mbi.Protect == PAGE_READWRITE;
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
        return 1;
    }
    return 0;
}
/* 0x421ff0, x86 call at the end of the n-byte window p (code bytes): 1 = a direct near call E8, its
   32-bit relative target plus base stored to *target; 2 = an indirect FF /2 call (register or memory,
   including the FF 15 disp32 IAT form); 0 = no call */

char err_CheckCall(char *p, int n, int base, int *target)
{
    int *rel;

    *target = 0;
    if (n >= 5 && p[-5] == (char)0xe8) {
        rel = (int *)(p - 4);
        *target = *rel + base;
        return 1;
    }
    if (n >= 3 && p[-3] == -1 && p[-2] == 0x14)
        return 2;
    if (n >= 6 && p[-6] == -1 && p[-5] == 0x15)
        return 2;
    if (n >= 2 && p[-2] == -1 && (unsigned char)p[-1] < 0x40)
        return 2;
    if (n >= 2 && p[-2] == -1 && (p[-1] & 0xf8) == 0xd0)
        return 2;
    if (n >= 3 && p[-3] == -1 && (p[-2] & 0xf8) == 0x50)
        return 2;
    if (n >= 7 && p[-7] == -1 && (p[-6] & 0xf8) == 0x90)
        return 2;
    return 0;
}
/* 0x4220b0, starts the error handling: stores the flags, the window and the crash callback, installs
   err_ExceptionFilter and opens the log files of the enabled channels */

void err_Init(int started, void (*crashCB)(HANDLE), HWND hwnd, unsigned int flags)
{
    char title[256];
    char path[260];
    time_t t;
    FILE *f;
    int i;

    err_Started = started;
    err_Flags = flags;
    err_CrashCB = crashCB;
    err_Hwnd = hwnd;
    err_ShowUnknown = (flags >> 4) & 1;
    SetUnhandledExceptionFilter(err_ExceptionFilter);
    t = time(NULL);
    GetWindowText(hwnd, title, sizeof(title));
    GetModuleFileName(NULL, err_Dir, 260);
    *strrchr(err_Dir, '\\') = 0;
    sprintf(err_Buf, "* ------------------------------------------------------------ *\n[%s], Date: %s", title, ctime(&t));
    if (err_Flags & ERR_LOG) {
        for (i = 0; i < 12; i++) {
            sprintf(path, "%s\\%s", err_Dir, err_Names[i]);
            f = fopen(path, "at");
            if (f) {
                fseek(f, 0, SEEK_END);
                if (ftell(f) > 0x10000) {
                    fclose(f);
                    f = fopen(path, "wt");
                    fputs("LogFile has been deleted!\n", f);
                }
                fclose(f);
            }
        }
    }
    if (err_Flags & ERR_CONSOLE) {
        AllocConsole();
        err_Console = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    for (i = 0; i < 12; i++)
        err_Write(err_Buf, err_Flags, err_Channels[i]);
}
/* 0x422250, closes the error handling: reports it in every channel's log, frees the console and
   clears the state */

void err_Exit(void)
{
    int i;

    for (i = 0; i < 12; i++)
        err_Write("Error-Handler closed.\n", err_Flags, err_Channels[i]);
    if (err_Flags & ERR_CONSOLE) {
        FreeConsole();
        err_Console = 0;
    }
    err_Started = 0;
    err_Hwnd = 0;
    err_Flags = 0;
}
/* 0x4222b0, reports a plain message: logs it in the on channels and shows it in a message box when
   ERR_MSGBOX is set; warns when the error handler has not been started */

void err_Message(char *text)
{
    if (!err_Started) {
        MessageBox(NULL, "ERROR-Handler has not been started...", "Error!", 0x10030);
        return;
    }
    if (err_Flags) {
        if (err_Flags & (ERR_LOG | ERR_DEBUG | ERR_CONSOLE)) {
            sprintf(err_Buf, "MESSAGE: %s\n", text);
            err_Write(err_Buf, err_Flags, 1);
        }
        if (err_Flags & ERR_MSGBOX)
            MessageBox(NULL, text, "Message:", 0x1030);
    }
}
/* 0x422330, writes a "Module '<file>': Line:<line>: <text>" line (with the time when ERR_TIME is
   set) to one channel log; nothing unless the error handler is started and logging is on */

void err_Log(char *file, int line, char *text, int channel)
{
    if (!err_Started) {
        MessageBox(NULL, "ERROR-Handler has not been started...", "Error!", 0x10030);
        return;
    }
    if (err_Flags & ERR_LOG) {
        if (err_Flags & ERR_TIME)
            sprintf(err_Buf, "Time:%8lu\tModule '%s':\tLine:%lu:\t%s", timeGetTime(), file, line, text);
        else
            sprintf(err_Buf, "tModule '%s':\tLine:%lu:\t%s", file, line, text);
        err_Write(err_Buf, err_Flags, channel);
    }
}
