/* 0x4aff30, text command: sends message 0x7d (arg 1) for the character ai_PlayerChar, today 8:00 */

int ai_Unk4aff30(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7d;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    TMX(msg) = NOWX;
    msg.fromId = -1;
    msg.flags = 1;
    msg.car = 1;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}
