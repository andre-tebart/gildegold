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
