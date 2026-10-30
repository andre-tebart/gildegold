/* 0x4904c0, execute of method 0x89 */

int ai_Unk4904c0(AIPX *p, AIBLD *b, int unused, AIARG *o)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x89;
    msg.flags = 1;
    msg.to = p->netId;
    msg.fromId = b->id;
    msg.time = ai_TimeT;
    msg.arg[1] = o->unk0c;
    msg.arg[2] = o->unk04;
    msg.ids[0] = o->unk04;
    msg.arg[3] = o->unk10;
    msg.arg[0] = o->unk08;
    cm_SendLongMessage(&msg);
    return 0x3d;
}
