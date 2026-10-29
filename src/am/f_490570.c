/* 0x490570, decide of method 0x8a */

int ai_Unk490570(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    AIPX *c;

    o1->unk10 = 3;
    o1->unk0c = b->id;
    if (!gm_Query(b->objList, 2, 0, 0x1fb, 6))
        return 0;
    c = ai_Unk48feb0(b);
    if (!c)
        return 0;
    if (he_FindEvent(2, 0, 0x8a, 3, b->id))
        return 0;
    o2->unk04 = c->netId;
    return util_Random(3) == 0;
}
