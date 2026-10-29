/* 0x4c9450, handler of type 0x2b: finds the object data[0] as one of three views (gm_FindById) and calls
   gm_RemoveOwned on its list with data[8] */

int ai_Unk4c9450(G17MSG *m, G17STAT *s)
{
    char *a, *b, *c;
    int r;

    if (s != 0)
        s->state = 2;
    gm_FindById(&a, &b, &c, G17_I(*m, 0));
    if (a != 0) {
        if (G17_I(*m, 4) == -1)
            r = gm_RemoveOwned(a + 0x5d, G17_I(*m, 8));
        else {
            b = gm_Query(*(int *)(a + 0x5d), 2, 6, 1, G17_I(*m, 4));
            if (b == 0)
                return -2;
            r = gm_RemoveOwned(b + 0x14, G17_I(*m, 8));
        }
    } else if (c != 0)
        r = gm_RemoveOwned(c + 0x178, G17_I(*m, 8));
    else if (b != 0)
        r = gm_RemoveOwned(b + 0x14, G17_I(*m, 8));
    else
        return -1;
    if (r != 0)
        return r;
    if (s != 0)
        s->state = 1;
    return 0;
}
