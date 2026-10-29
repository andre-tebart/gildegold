/* 0x4aab90, which of b and c character p trusts more (distance 0.66, 0x504ec0 0.34, item 0x2be +0.2): 1 b, 0 c,
   2 neither; *k 2 if the difference is small, else 1 (the middle part is a rough draft) */

int ai_TrustedOfTwo(AIPER *p, AIPER *b, AIPER *c, int *k)
{
    float db, wb, dc, wc, x1, x2, fb, fc;

    *k = 0;
    if (p == c)
        return 0;
    if (p == b)
        return 1;
    db = gm_Liking(b->id, p->id, 1);
    wb = gm_GetSkill(b, 4);
    dc = gm_Liking(c->id, p->id, 1);
    wc = gm_GetSkill(c, 4);
    x1 = db * 0.66 + wb * 0.34;
    x2 = dc * 0.66 + wc * 0.34;
    fb = wb;
    if (ai_InventoryHas(b, 0x2be) != 0)
        fb += 0.2f;
    fc = wc;
    if (ai_InventoryHas(c, 0x2be) != 0)
        fc += 0.2f;
    if (abs((int)(fb - fc)) * 0.66 <= abs((int)(x1 - x2)) * 0.33)
        *k = 2;
    else
        *k = 1;
    if (x2 < x1 - 5.0f)
        return 1;
    if (x1 < x2 - 5.0f)
        return 0;
    return 2;
}
