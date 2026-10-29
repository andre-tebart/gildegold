/* 0x48edc0, execute, way 2: pays the amount of s2 and the amount of s1 */

int ai_Unk48edc0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    union { struct { unsigned int lo, hi; } s; __int64 q; } m;
    AIMSG msg;
    AIPX *c;
    int v;

    c = (AIPX *)gm_FindChar(s1->unk0c);
    if (!c)
        return 0;
    cm_SendMoveGoodsUnchecked(-1, p->netId, ai_OwnPlayer, s2->unk08);
    gm_SendMoneyDelta(p->netId, -(int)s1->unk08);
    memset(&msg, 0, sizeof(msg));
    msg.to = p->netId;
    TMX(msg) = NOWX;
    msg.fromId = s1->unk0c;
    msg.type = 0x99;
    msg.flags = 2;
    msg.ch = c->netId;
    msg.dst[1] = 2;
    msg.dst[2] = s2->unk10;
    msg.dst[0] = -1;
    msg.n = msg.fromId;
    if (c->bld170)
        msg.car = c->bld170->id;
    else if (c->bld16c)
        msg.car = c->bld16c->id;
    else {
        v = -1;
        m.s.lo = 0x7b4c4a0;
        m.s.hi = 0;
        ((int (*)(AIPX *, int, __int64, int, float, int *))ai_Unk4a63f0)(c, 0, m.q, 0, 100.0f, &v);
        msg.car = v;
    }
    cm_SendLongMessage(&msg);
    return 1;
}
