/* 0x4cbff0, handler of type 0x4d */

int ai_Unk4cbff0(G17MSG *m, G17STAT *s)
{
    char *c, *g;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    g = *(char **)(c + 0x184);
    if (g != 0 && *(int *)(g + 0x28) != -1)
        evt_Stop(evt_FindById(*(int *)(g + 0x28)));
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
