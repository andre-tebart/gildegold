/* 0x4cab40, handler of type 0x39: building data[0] gets the character data[4] as its boss (+0x170) */

int ai_Unk4cab40(G17MSG *m, G17STAT *s)
{
    unsigned short n;
    char *b, *c, *o;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    c = gm_FindChar(G17_I(*m, 4));
    if (c == 0 && G17_I(*m, 4) != -1)
        return 1;
    if (ai_BTypes[(unsigned char)b[0] * 0x24d] == 1) {
        ord_GetKindRecord((unsigned char)b[0], &n);
        if ((unsigned char)b[0x65] >= n)
            return 1;
        b[0x65]++;
    }
    o = *(char **)(c + 0x170);
    if (o != 0) {
        if (ai_BTypes[(unsigned char)o[0] * 0x24d] == 1 && (unsigned char)o[0x65] > 0)
            o[0x65]--;
    }
    *(char **)(c + 0x170) = b;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
