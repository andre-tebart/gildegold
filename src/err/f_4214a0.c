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
