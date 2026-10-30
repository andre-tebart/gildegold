/* 0x4ab9f0, execute callback of action 0x45 "Verfluchen": p is flagged (0x1cc bit 4), pays, and the curse goes
   out (message 0x9d: strength, victim) */

int ai_Unk4ab9f0(AIPER *p, unsigned char *m)
{
    AIMSG msg;

    cm_SendSetField(p->netId, 0x1cc, 4, 0x10, 0);
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, -*(int *)(m + 0x10));
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.time = ai_TimeT;
    msg.type = 0x9d;
    msg.fromId = -1;
    msg.flags = 2;
    msg.arg[0] = *(int *)(m + 0xc);
    msg.arg[1] = *(int *)(m + 4);
    cm_SendLongMessage(&msg);
    return 0x45;
}
