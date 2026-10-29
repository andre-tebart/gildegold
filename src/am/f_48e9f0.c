/* 0x48e9f0, decide helper: level 3, the master pays from his own purse */

int ai_Unk48e9f0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    int s, m, lim, sel, a[3];

    s = gm_GetWorth(p->id);
    m = gm_Unk509e30(p->inventory, ai_OwnPlayer);
    lim = (int)(m * 0.25f);
    a[0] = (int)(s * 0.01f);
    a[1] = (int)(s * 0.015f);
    a[2] = (int)(s * 0.02f);
    if (lim > a[2])
        sel = 2;
    else if (lim > a[1])
        sel = 1;
    else if (lim > a[0])
        sel = 0;
    else
        return 0;
    s1->unk10 = 3;
    s2->unk10 = sel;
    s2->unk08 = a[sel];
    return 1;
}
