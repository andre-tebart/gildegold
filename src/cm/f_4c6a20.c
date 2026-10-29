/* 0x4c6a20, handler of the types 0, 3, 5, 6, 7 (nothing to do); 1 for any other type */

int ai_Unk4c6a20(G17MSG *m, G17STAT *s)
{
    if (m->type != 0 && m->type != 5 && m->type != 6 && m->type != 7)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
