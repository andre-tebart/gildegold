/* 0x4b1600, sends message 0x57 to every player (0x4ae030), today at 10..13 o'clock (by id) */

int ai_Unk4b1600(void)
{
    AITIME t;
    AIMSG msg;
    int i;

    *(AITIMEX *)&t = NOWX;
    memset(&msg, 0, sizeof(msg));
    t.min = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingPlayer((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            t.hour = i % 4 + 10;
            msg.type = 0x57;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 1;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
