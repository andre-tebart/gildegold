/* 0x494410, decides whether a plan between two characters loses against a third */

int ai_Unk494410(AIC4 *ch, AIPLAN *q)
{
    AIC4 *a, *b;
    float x, y, c, d;

    if (q->unk48 != 1 && q->unk08 != 1) {
        a = AI_C4(q->c18);
        b = AI_C4(q->c14);
        x = gm_Liking(a->id, ch->id, 1) * 0.01f;
        y = gm_Liking(b->id, ch->id, 1) * 0.01f;
        if (ai_InventoryHas(a, 0x22e)) {
            x = x + 0.1f;
            y = y - 0.1f;
        }
        x = min(max(0.0f, x), 1.0f);
        y = min(max(0.0f, y), 1.0f);
        c = gm_Mix(a, ch, 4);
        d = gm_Mix(b, ch, 4);
        d = d + q->unk04 * q->unk04 * 0.0025f;
        if (q->unk44) {
            x = x * 1.1f;
            y = y * 0.9f;
        }
        if (c + x > d + y)
            return 1;
    }
    return 0;
}
