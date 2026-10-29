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
