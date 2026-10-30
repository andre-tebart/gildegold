/* 0x48e830, decide helper: pay in three steps, level 2 */

int ai_Unk48e830(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, sel, sel2, sum, a[6];
    float f;
    AIPX *c;

    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (!ai_Unk4a58d0(p, 1, &flags, 0, util_Random(0x20) + 32.0f, &cid))
        return 0;
    c = &((AIPX *)ai_CharsT)[cid];
    f = (float)(amt_GetKindLevel(((unsigned char *)c)[0x166]) / 3 + 2);
    a[0] = (int)f;
    a[1] = (int)(f * 1.5f);
    a[2] = (int)(f + f);
    if (p->unk194 >= a[2])
        sel = 2;
    else if (p->unk194 >= a[1])
        sel = 1;
    else if (p->unk194 >= a[0])
        sel = 0;
    else
        return 0;
    gm_Unk509e30(p->inventory, ai_OwnPlayer);
    sum = gm_GetWorth(p->id) + gm_GetWorth(c->id);
    a[3] = (int)(sum * 0.01f);
    a[4] = (int)(sum * 0.015f);
    a[5] = (int)(sum * 0.02f);
    if (sum >= a[2])
        sel2 = 2;
    else if (sum >= a[1])
        sel2 = 1;
    else if (sum >= a[0])
        sel2 = 0;
    else
        return 0;
    if (sel2 >= sel)
        sel2 = sel;
    s2->unk08 = a[sel2 + 3];
    s1->unk08 = a[sel2];
    s2->unk0c = c->netId;
    s2->unk10 = sel2;
    s1->unk10 = 2;
    return 1;
}
