/* 0x4cc050, handler of type 0x4e: person data[0] (with s: walks there, else the action 0x3a) */

int ai_Unk4cc050(G17MSG *m, G17STAT *s)
{
    char *p, *q = 0, *r;
    int sc;

    p = ch_FindByName((char *)&m->data[0]);
    if (p == 0)
        return 1;
    if (m->data[0] != 0)
        q = ch_FindByName((char *)&m->data[0x20]);
    sc = d3t_SceneIndex(*(int *)(p + 0x88));
    if (s != 0) {
        r = ch_AddAction(p, 0x2d, G17_I(*m, 0x18), G17_I(*m, 0x1c), sc);
        *(int *)(r + 0x160) |= 2;
        if (q != 0) {
            strcpy(r + 0x114, (char *)&m->data[0x20]);
            *(int *)(r + 0x160) |= 8;
        }
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
        return 0;
    }
    ch_AddAction(p, 0x3a, G17_I(*m, 0x18), G17_I(*m, 0x1c), sc);
    return 0;
}
