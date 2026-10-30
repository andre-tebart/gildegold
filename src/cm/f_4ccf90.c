/* 0x4ccf90, handler of type 0x63 */

int ai_Unk4ccf90(G17MSG *m, G17STAT *s)
{
    int id;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    else
        G17_I(*m, 0) = rand();
    util_Seed(G17_I(*m, 0));
    id = ahm_CreateAlchemistI((int *)&m->data[0]);
    if (id == 0xffff)
        return -1;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
