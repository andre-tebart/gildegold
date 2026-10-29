/* 0x4abc60, execute callback of action 0x46 "Theater": p pays and puts on the play in building m + 4
   (message 0xa0, length m + 0xc) */

int ai_Unk4abc60(AIPER *p, unsigned char *m)
{
    AIMSG msg;

    gm_Unk50cc50(p->netId, -*(int *)(m + 0x10));
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    msg.fromId = *(int *)(m + 4);
    msg.time = ai_Time;
    msg.type = 0xa0;
    msg.flags = 2;
    msg.arg[0] = *(int *)(m + 0xc);
    cm_SendLongMessage(&msg);
    return 0x46;
}
