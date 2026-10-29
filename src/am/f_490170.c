/* 0x490170, decide of method 0x90 */

int ai_Unk490170(AIPX *p, AIBLD *b, AIARG *o1, AIARG *o2)
{
    unsigned short u1, u2;
    unsigned int n;
    int x;

    u1 = util_Random(3);
    u2 = util_Random(3);
    o1->unk10 = 1;
    o1->unk0c = b->id;
    n = (u2 + 2) * (u1 + 2);
    if (n > (unsigned int)p->unk194)
        return 0;
    if (!gm_Query(b->objList, 2, 0, 0x211, 6))
        return 0;
    if (he_FindEvent(2, 0, 0x90, 3, b->id))
        return 0;
    x = -1;
    if (ai_Partner) {
        AI_4A63F0_7(ai_Partner, 4, 0xa07c43b4, 1, 0, 100.0f, &x);
        if (x != -1)
            goto done;
    }
    if (!AI_4A63F0_7(p, 6, 0xa07c43b4, 1, 0, util_Random(0x20) + 19.0f, &x))
        return 0;
done:
    o2->unk08 = u2;
    o2->unk04 = u1;
    o2->unk0c = x;
    return 1;
}
