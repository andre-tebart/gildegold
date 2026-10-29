/* 0x4b1bd0, text command "-n": message 0x59 with n clamped to 1..10, today 11:00 */

int ai_Unk4b1bd0(int a, int b, char *fmt)
{
    AIMSG msg;
    int n;

    if (*fmt != '-')
        return 0;
    n = atoi(fmt + 1);
    if (n <= 10) {
        if (n <= 1)
            n = 1;
    } else
        n = 10;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x59;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    msg.car = n;
    msg.fromId = -1;
    msg.flags = 2;
    TMX(msg) = NOWX;
    msg.time.hour = 11;
    cm_SendLongMessage(&msg);
    return 1;
}
