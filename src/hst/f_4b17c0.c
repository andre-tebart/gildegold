/* 0x4b17c0, text command "-<d>..." (d one of N S O W A): message 0x51 with n = atoi(fmt + 1) (sic: starts at d)
   and d, if n > 0 */

int ai_Unk4b17c0(int a, int b, char *fmt)
{
    AIMSG msg;
    char *p;
    char d;
    int n;

    if (*fmt != '-')
        return 0;
    p = fmt + 1;
    if (strlen(p) < 2)
        return 0;
    d = *p;
    if (d != 'N' && d != 'S' && d != 'O' && d != 'W' && d != 'A')
        return 0;
    n = atoi(p);
    if (n > 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x51;
        msg.to = *(int *)((char *)ai_PlayerChar + 4);
        msg.fromId = -1;
        msg.flags = 0x12;
        *((char *)&msg + 0x58) = (char)n;
        *((char *)&msg + 0x59) = d;
        cm_SendLongMessage(&msg);
    }
    return 1;
}
