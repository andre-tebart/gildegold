/* 0x4b1880, sends message 0x54 to the first 8 characters of 0x4ae0d0, today 12:00 */

int ai_Unk4b1880(void)
{
    AITIME t;
    AIMSG msg;
    int i, n;

    *(AITIMEX *)&t = NOWX;
    t.hour = 12;
    t.min = 0;
    n = 0;
    for (i = 0; i < 0x300 && n < 8; i++) {
        if (ai_IsLivingHuman((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x54;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 9;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
            n++;
        }
    }
    return 1;
}
