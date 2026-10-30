/* 0x48e730, decide helper: pay in three steps, level 1 */

int ai_Unk48e730(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, t, sel, a[3];
    AIPX *c;

    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (!ai_Unk4a58d0(p, 1, &flags, 0, util_RandomU(0x20) + 25.0f, &cid))
        return 0;
    c = &((AIPX *)ai_CharsT)[cid];
    t = amt_GetKindLevel(((unsigned char *)c)[0x166]) / 3 + 2;
    a[0] = t + 3;
    a[1] = t + 5;
    a[2] = t + 8;
    if (p->unk194 >= a[2])
        sel = 2;
    else if (p->unk194 >= a[1])
        sel = 1;
    else if (p->unk194 >= a[0])
        sel = 0;
    else
        return 0;
    s1->unk08 = a[sel];
    s2->unk0c = c->netId;
    s2->unk10 = sel;
    s1->unk10 = 1;
    return 1;
}
