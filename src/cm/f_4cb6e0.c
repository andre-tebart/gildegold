/* 0x4cb6e0, handler of type 0x3f: a 0x80-byte record (id byte data[0], word data[1]) of gm_FindGoods is replaced */

int ai_Unk4cb6e0(G17MSG *m, G17STAT *s)
{
    char *r;

    r = gm_FindGoods(m->data[0], *(unsigned short *)&m->data[1]);
    if (r == 0)
        return 1;
    memcpy(r, &m->data[1], 0x80);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
