/* 0x4902b0, execute of method 0x90 */

int ai_Unk4902b0(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x90;
    msg.flags = 2;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_Time;
    msg.arg[1] = o->unk04;
    msg.arg[2] = o->unk08;
    msg.arg[0] = o->unk0c;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
