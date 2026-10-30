/* 0x4afe80, text command: sends message 0x7f to every councillor (kind 6 / 7), today 8:00 */

int ai_Unk4afe80(void)
{
    AIMSG msg;
    int i;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7f;
    TMX(msg) = NOWX;
    msg.fromId = -1;
    msg.time.hour = 8;
    msg.time.min = 0;
    msg.flags = 1;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7) {
            msg.to = ai_CharsT[i].netId;
            msg.car = ai_CharsT[i].netId;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
