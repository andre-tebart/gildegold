/* 0x48ff50, decide of method 0x88 */

int ai_Unk48ff50(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c, *q;
    unsigned short id;
    int flags, x;

    o1->unk10 = 0;
    o1->unk0c = b->id;
    flags = 0;
    if (p->unk194 < 4)
        return 0;
    if (!gm_Query(b->objList, 2, 0, 0x1fc, 6))
        return 0;
    c = ai_Unk48feb0(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x88, 3, b->id))
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (!ai_Unk4a58d0((AIPER *)p, 1, &flags, 0, util_RandomU(0x20) + 32.0f, &id))
        return 0;
    q = &AIC[id];
    if (q->bld2)
        o2->unk08 = q->bld2->id;
    else if (q->bld)
        o2->unk08 = q->bld->id;
    else {
        x = -1;
        AI_4A63F0_7(p, 0, 0x7b4c4a0, 0, 0, 100.0f, &x);
        if (x == -1)
            return 0;
        o2->unk08 = x;
    }
    o2->unk0c = q->netId;
    o2->unk04 = c->netId;
    o2->unk10 = 4;
    return 1;
}
