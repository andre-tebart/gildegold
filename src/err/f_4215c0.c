/* 0x4215c0, unhandled exception filter: writes <exe>.dbg-symbolized crash info to _crashdump.log (ROUGH) @deep behaviour: ROUGH draft, 764 code hunks; the original function ends at 0x421c34 (ret 4) and the bytes after are its switch jump table, which asmdiff reads as code (the inc/sbb/xchg noise at 421c34+); our object is 0x3a bytes shorter, mainly because the original inlines the extension scan (forward to the NUL, back to '.', then 'd','b','g',0 at 421600-421647) where our source calls strrchr (push 0x2e/push path/call); the exception-name switch bodies are laid out in another order (the WriteFile length pushes pair up across cases), the rest is frame 0x28c vs 0x280 and the 0x10 slot shift / register allocation; readability: Q2 err_CrashCB (the global crash hook at 0x63236c, set by err_Init, called with the log handle on a crash and -1 otherwise), Q4 one raw '*(unsigned int *)map' (the map key), Q5 ROUGH; needs a source rewrite and the byte toolbox */

LONG WINAPI err_ExceptionFilter(EXCEPTION_POINTERS *ep)
{
    char handled = 0;
    CONTEXT *ctx = ep->ContextRecord;
    char path[256];
    char text[256];
    char crlf[] = "\r\n";
    char sp[] = " ";
    char sep[] = "------------------------\r\n";
    char win[7];
    MEMORY_BASIC_INFORMATION mbi;
    ERRWALK w;
    HANDLE mapfile, log, proc;
    char *map;
    unsigned int size, got, key, *p, i, *stack;
    int remaining, len;
    DWORD n;
    char *dot;

    SetUnhandledExceptionFilter(NULL);
    if (GetModuleFileName(NULL, path, sizeof(path))) {
        dot = strrchr(path, '.');
        if (dot) {
            dot[1] = 'd';
            dot[2] = 'b';
            dot[3] = 'g';
            dot[4] = 0;
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
                        key = *(unsigned int *)map;
                        p = (unsigned int *)(map + 0x10);
                        for (i = 0; i < (size >> 2) - 4; i++) {
                            key = (key << 15) | (key >> 17);
                            *p++ ^= key;
                        }
                        if (log != INVALID_HANDLE_VALUE) {
                            proc = GetCurrentProcess();
                            remaining = 0x800;
                            stack = (unsigned int *)ctx->Esp;
                            SetFilePointer(log, -1, NULL, FILE_END);
                            WriteFile(log, sep, 0x1a, &n, NULL);
                            len = GetDateFormat(0x400, 2, NULL, NULL, text, 0x100);
                            if (len) {
                                WriteFile(log, text, len - 1, &n, NULL);
                                WriteFile(log, sp, 1, &n, NULL);
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
                            ERR_EXC(EXCEPTION_NONCONTINUABLE_EXCEPTION, "NONCONTINUABLE_EXCEPTION");
                            ERR_EXC(EXCEPTION_INVALID_DISPOSITION, "INVALID_DISPOSITION");
                            ERR_EXC(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "ARRAY_BOUNDS_EXCEEDED");
                            ERR_EXC(EXCEPTION_FLT_DENORMAL_OPERAND, "FLT_DENORMAL_OPERAND");
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
                            w.map = (ERRMAP *)map;
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
                                        if (!err_CheckCall(win + 7, len, w.addr, (int *)&w.prev))
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
