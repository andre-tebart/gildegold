/* 0x494020, decides whether a plan of a character is worth it */

/* ---- 0x494020..0x494e70 (was unk_494020.c): decision / rating callbacks that compare two characters
   (closeness, rank, random rolls), a plan's tries, pair and group ratings ---- */

int ai_Unk494020(AIPLAN *q)
{
    AIC4 *a, *b, *c;
    unsigned int n;
    float lim, x, d, sum, rank;

    if (q->unk04 <= 3.0f)
        return 0;
    if (q->unk04 >= 18.0f)
        return 1;
    n = ai_Unk4942a0(q);
    a = AI_C4(q->c18);
    b = AI_C4(q->c1c);
    c = AI_C4(q->c14);
    sum = (float)a->unk024 + a->unk01c;
    rank = a->unk10c - 2.0f;
    if (rank > 0.66f)
        lim = (float)(n * 190);
    else if (rank > 0.33f)
        lim = (float)(n * 90);
    else
        lim = (float)(n * 70);
    if (lim > sum)
        return 1;
    x = gm_Liking(a->id, b->id, 1) * 0.01f;
    d = gm_Liking(c->id, b->id, 1) * 0.01f;
    if (x * 1.1f > d * 0.9f && x < d)
        return 0;
    return util_RandomU(2);
}
