/* 0x4ccc20, handler of type 0x5d: adds data[8] to the skill byte data[4] (0..4) of character data[0] (0..252) */

int ai_Unk4ccc20(G17MSG *m, G17STAT *s)
{
    char *c;
    int k;
    float f;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    k = G17_I(*m, 4);
    if (k < 0 || k > 4)
        return 1;
    f = (float)((unsigned char)c[0x80 + k] + G17_I(*m, 8));
    f = f >= 252.0f ? 252.0f : (f <= 0.0f ? 0.0f : f);
    c[0x80 + k] = (char)f;
    if (s != 0)
        s->state = 1;
    return 0;
}
