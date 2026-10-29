/* 0x48e620, decide helper: a character to pay for the upgrade, level 0 */

int ai_Unk48e620(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    unsigned short cid;
    int flags, lim, cost, m;
    AIPX *c;

    if (p->unk194 < 0x10)
        return 0;
    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    if (!ai_Unk4a58d0(p, 1, &flags, 0, util_Random(0x20) + 25.0f, &cid))
        return 0;
    c = &((AIPX *)ai_Chars)[cid];
    m = gm_Unk509e30(p->inventory, ai_OwnPlayer);
    lim = (int)(m * 0.25f);
    {
        int s1v = gm_GetWorth(p->id);
        cost = (int)((gm_GetWorth(c->id) + s1v) * 0.02f);
    }
    if (lim < cost)
        return 0;
    s2->unk08 = cost;
    s1->unk08 = 0;
    s2->unk0c = c->netId;
    s2->unk10 = 0;
    s1->unk10 = 0;
    return 1;
}
