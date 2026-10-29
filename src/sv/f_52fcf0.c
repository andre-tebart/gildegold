/* 0x52fcf0, sends the text t as a network message (0x4c4db0 (0xe, copy)) and shows it as the status line (0x52e330) until the answer arrives; clears the status line afterwards */

void sv_BroadcastText(unsigned fl, char *t)
{
    char msg[0x7c];
    int m;

    memset(msg, 0, sizeof(msg));
    strcpy(msg, t);
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    while (cm_GetSlotState(m) == 0) {
        sv_SetStatusText(t);
        sv_RunFrame(0, fl | 0x300000);
    }
    sv_SetStatusText(ch_EmptyName);
}
