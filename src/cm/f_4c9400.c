/* 0x4c9400, handler of type 0x2a */

int ai_Unk4c9400(G17MSG *m, G17STAT *s)
{
    if (cm_PartyById(G17_I(*m, 0)) == 0)
        return 1;
    if (ai_Unk4e5210(G17_I(*m, 0), G17_I(*m, 4)) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
