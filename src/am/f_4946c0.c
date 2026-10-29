/* 0x4946c0, the rank of 0x4945c0 raised by how close the character is (0..9) */

int ai_Unk4946c0(AIC4 *ch, AIPLAN *q)
{
    int n;
    float k, d;
    AIPOS pos;
    AIC4 *c;

    n = ai_Unk4945c0(q);
    gv_GetTableRecord(0, &pos);
    k = (5.0f - (float)pos.unk18) * 0.1f + 0.7f;
    c = AI_C4(q->c18);
    d = gm_Liking(c->id, ch->id, 1);
    if (!(d > k * 66.666f)) {
        if (d > k * 33.333f)
            n += 1;
        else
            n += 2;
    }
    if (n == 5) {
        if (c->unk166 == 0)
            n = 4;
    } else if (n == 7 && c->unk00d < 2)
        n = 6;
    n = n <= 0 ? 0 : n;
    if (n >= 9)
        n = 9;
    return n;
}
