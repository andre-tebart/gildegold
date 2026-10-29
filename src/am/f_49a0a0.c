/* 0x49a0a0, execute callback: a public announcement, every councillor (kind 6 / 7) is told */

int ai_Unk49a0a0(G5CH *p)
{
    G8MSG m;
    AIBLD *b;
    AIOBJ *o;
    unsigned char *c;
    AITIME t;
    int n;
    int v;

    b = gm_BuildQuery(1, 5, 0xf);
    v = 2;
    if (b == 0)
        return 0;
    o = gm_Query(*(int *)((char *)b + 0x5d), 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    G8_4C4830I(*(int *)((char *)o + 2), p->netId, 0, 16000);
    cm_BeginFieldBlock((AIPER *)p, p->netId);
    cm_AddFieldDelta(1, 1, 0xd, &v);
    cm_SendFieldDeltas();
    cm_SendSetField(p->netId, 0x1c8, 4, 0, 0x4000);
    t = ai_Time;
    gm_AddTime(&t, util_Random(4) + 1, util_Random(8) * 7, 0);
    if (((unsigned char *)p)[2] == 5) {
        n = 0;
        for (c = (unsigned char *)ai_Chars + 2; c < (unsigned char *)(ai_Chars + 768) + 2; c += 0x21c) {
            if (n >= (int)ai_NumPlayers)
                return 0x19;
            if (c[0] == 6 || c[0] == 7) {
                AI_ZERO(&m, 0xf8);
                m.to = *(unsigned int *)(c + 2);
                m.time = t;
                m.from = p->netId;
                m.type = 0x69;
                m.unk0c = -1;
                m.unk36 = 9;
                m.unk58 = 2;
                cm_SendLongMessage((AIMSG *)&m);
                n++;
            }
        }
    }
    return 0x19;
}
