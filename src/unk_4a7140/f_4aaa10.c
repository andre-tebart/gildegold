/* 0x4aaa10, execute callback "aip_ExecDrohen": p threatens the character of m (message 0x2e, by chance after
   0x504e20) */

int ai_Unk4aaa10(AIPER *p, int c, unsigned char *m)
{
    AIMSG msg;
    AIPER *o;

    if (m[0] != 7)
        return 0;
    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    cm_BeginGroup("aip_ExecDrohen");
    if (util_RandomF() + gm_Mix(p, o, 4) >= 1.0f) {
        msg.type = 0x2e;
        msg.to = o->netId;
        msg.fromId = -1;
        msg.flags = 2;
        msg.arg[1] = p->netId;
        msg.arg[0] = o->netId;
        cm_SendLongMessage(&msg);
    }
    cm_SendMoneyDelta(p->netId, -2);
    cm_EndGroup("aip_ExecDrohen");
    return 1;
}
