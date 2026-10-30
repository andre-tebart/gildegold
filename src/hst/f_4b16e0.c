/* 0x4b16e0, the same with message 0x58 at 13..16 o'clock */

int ai_Unk4b16e0(void)
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
            t.hour = i % 4 + 13;
            msg.type = 0x58;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 1;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
