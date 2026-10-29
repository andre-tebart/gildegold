/* 0x48ef10, execute, way 3: pays the amount of s2, message to nobody */

int ai_Unk48ef10(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    int v;

    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->unk08);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    TMX(msg) = NOWX;
    msg.fromId = s1->unk0c;
    msg.dst[2] = s2->unk10;
    msg.type = 0x99;
    msg.flags = 2;
    msg.ch = -1;
    msg.dst[0] = -1;
    msg.dst[1] = 3;
    v = -1;
    msg.n = msg.fromId;
    m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ((int (*)(AIPX *, int, __int64, int, float, int *))ai_Unk4a63f0)(p, 0, m.q, 0, 100.0f, &v);
    msg.car = v;
    cm_SendLongMessage(&msg);
    return 1;
}
