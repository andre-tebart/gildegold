/* 0x52fd80, like 0x52fcf0 with the key mask 0x60d360 | 0x210000 and no clearing of the status line */

void sv_Unk52fd80(char *t)
{
    char msg[0x7c];
    int m;

    memset(msg, 0, sizeof(msg));
    strcpy(msg, t);
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    while (cm_GetSlotState(m) == 0) {
        sv_SetStatusText(t);
        sv_RunFrame(0, ai_Unk60d360 | 0x210000);
    }
}
