/* 0x4caa20, handler of type 0x37 */

int ai_Unk4caa20(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 2;
    ch_QueueAnim(p, &m->data[4], m->data[0x34]);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
