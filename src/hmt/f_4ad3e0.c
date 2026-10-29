/* 0x4ad3e0, the election score of candidate c for the office of obj: relation to obj's holder (0x50c550, * 0.01,
   twice if `twice`), (age byte + 1) * 0.1 * 2, and the two titles (0x166 / 0x167) weighted against title 0x1b
   @deep code 107 + slots 16 (71 ours / 74 original instructions, calls 4 = 4): the original's code is the same expression - rel = 0x50c550(c->id, obj's word, 1) * 0.01 doubled when twice, age = (byte +0xd + 1) * 0.1 doubled, w1/w2 = 0x4e7c40(title)/0x4e7c40(0x1b), t = 0x4ea5c0(c, 0x4e6f10(title)), return t1*w1 + t2*w2 + age + rel - with the four float locals in other stack slots and the x87 stack in another order (fild/fld/faddp positions differ); no constant, callee or statement differs */

float hmt_Score(AICHAR *c, void *obj, int type, int twice)
{
    float rel, age, t1, t2, w1, w2;

    rel = gm_Liking(c->id, *(unsigned short *)obj, 1) * 0.01f;
    if (twice)
        rel = rel + rel;
    age = (HMT_B(c, 0x0d) + 1) * 0.1f;
    w1 = (float)amt_GetKindLevel(c->officeKind) / (float)amt_GetKindLevel(0x1b);
    w2 = (float)amt_GetKindLevel(c->prevOffice) / (float)amt_GetKindLevel(0x1b);
    t1 = 0;
    if (c->officeKind)
        t1 = amt_GetDistrictLiking(c, amt_KindColumn(c->officeKind));
    t2 = 0.0f;
    if (c->prevOffice)
        t2 = amt_GetDistrictLiking(c, amt_KindColumn(c->prevOffice));
    return t1 * w1 + t2 * w2 + (age + age) + rel;
}
