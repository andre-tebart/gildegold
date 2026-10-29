/* 0x4b1950, the same with message 0x55 at 10:00 */

int ai_Unk4b1950(void)
{
    AITIME t;
    AIMSG msg;
    int i, n;

    *(AITIMEX *)&t = NOWX;
    t.hour = 10;
    t.min = 0;
    n = 0;
    for (i = 0; i < 0x300 && n < 8; i++) {
        if (ai_IsLivingHuman((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x55;
            msg.to = ai_Chars[i].netId;
            msg.fromId = ai_Chars[i].netId;
            msg.flags = 9;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
            n++;
        }
    }
    return 1;
}
