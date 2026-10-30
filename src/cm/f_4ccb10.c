/* 0x4ccb10, handler of type 0x5b: adds data[4] to the byte +0x1b1 of the character data[0] (0..0xff) */

int ai_Unk4ccb10(G17MSG *m, G17STAT *s)
{
    char *c;
    int v;

    c = gm_FindChar(G17_I(*m, 0));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (c == 0)
        return 1;
    v = (unsigned char)c[0x1b1] + G17_I(*m, 4);
    v = v < 0 ? 0 : v;
    if (v >= 0xff)
        v = 0xff;
    c[0x1b1] = v;
    v = (unsigned char)c[0x1b1] != 0 ? (unsigned char)c[0x1b1] : 0;
    c[0x1b1] = v;
    if (*(unsigned short *)c == ai_PlayerId && ai_GameState != 0 && (unsigned char)v != 0)
        sv_PopTo(ai_GameFrame);
    amt_ClearOfficesIfAll(c, G17_I(*m, 8));
    if (s != 0)
        s->state = 1;
    return 0;
}
