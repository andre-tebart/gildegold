/* 0x490600, execute of method 0x8a */

int ai_Unk490600(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x8a;
    msg.flags = 1;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_TimeT;
    msg.arg[0] = o->unk04;
    msg.ids[0] = o->unk04;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
