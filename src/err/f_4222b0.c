/* 0x4222b0 */

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
