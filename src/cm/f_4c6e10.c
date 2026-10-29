/* 0x4c6e10, handler of type 0xd: character b joins a (0x5064d0) */

int ai_Unk4c6e10(G17MSG *m, G17STAT *s)
{
    char *c, *y;
    int *x;
    int k;

    k = gm_Unk5064d0(*(int *)&m->data[0], *(int *)&m->data[4]);
    if (s != 0) {
        s->state = 1;
        if (k >= 0) {
            c = gm_FindChar(*(int *)&m->data[4]);
            if (unk_Unk4f2540(c, &y, &x) != 0)
                unk_Unk4f01d0(c, y, *x, gv_EntryIndex(*(int *)(y + 0x65), x));
            s->unk01 = 1;
            s->value = (int)ai_Chars[k];
        } else {
            s->unk01 = 0;
            s->value = 0;
        }
    }
    return 0;
}
