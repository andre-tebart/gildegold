/* 0x4aaae0, execute callback "aip_ExecAnschwaerzen": p denounces the character of m to the one at m + 8
   (message 0x2f) */

int ai_Unk4aaae0(AIPER *p, int c, unsigned char *m)
{
    AIMSG msg;

    msg.to = *(int *)(m + 8);
    msg.arg[0] = *(int *)(m + 4);
    msg.arg[1] = p->netId;
    msg.time = ai_TimeT;
    msg.type = 0x2f;
    msg.fromId = -1;
    msg.flags = 1;
    msg.arg[2] = msg.to;
    cm_BeginGroup("aip_ExecAnschwaerzen");
    cm_SendLongMessage(&msg);
    cm_SendMoneyDelta(p->netId, -3);
    cm_EndGroup("aip_ExecAnschwaerzen");
    return 1;
}
