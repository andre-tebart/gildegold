/* 0x4941e0 */

int ai_Unk4941e0(AIPLAN *q)
{
    AIC4 *a, *b, *c;
    float x, y;

    c = AI_C4(q->c28);
    if (!c)
        return 1;
    a = AI_C4(q->c18);
    b = AI_C4(q->c14);
    x = gm_Liking(a->id, c->id, 1);
    y = gm_Liking(b->id, c->id, 1);
    if (x - y > 20.0f)
        return 1;
    if (y - x > 20.0f)
        return 0;
    return util_Random(2);
}
