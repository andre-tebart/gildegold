/* 0x4aa940, execute callback "aip_ExecPamphlet": p spreads a pamphlet about the character of m (message 0x35) */

int ai_Unk4aa940(AIPER *p, int c, unsigned char *m)
{
    AIMSG msg;
    AIPER *o;

    if (m[0] != 7)
        return 0;
    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    gm_BuildQuery(1, 5, 10);
    *(AITIME *)&msg.arg[0] = ai_TimeT;
    msg.to = p->netId;
    msg.type = 0x35;
    msg.flags = 2;
    msg.fromId = msg.to;
    ((unsigned char *)&msg)[0x6d] = (unsigned char)util_RandomU(0x1d);
    msg.arg[4] = o->netId;
    cm_BeginGroup("aip_ExecPamphlet");
    cm_SendLongMessage(&msg);
    cm_EndGroup("aip_ExecPamphlet");
    return 1;
}
