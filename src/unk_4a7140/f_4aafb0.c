/* 0x4aafb0, execute callback: message 0x74 from p to o about the object at m */

int ai_Unk4aafb0(AIPER *p, AIPER *o, unsigned char *m)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x74;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = o->netId;
    msg.time = ai_Time;
    msg.arg[1] = *(int *)(m + 4);
    msg.arg[0] = msg.fromId;
    cm_SendLongMessage(&msg);
    gm_SendMoneyDelta(p->netId, -1);
    return 1;
}
