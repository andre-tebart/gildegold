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
