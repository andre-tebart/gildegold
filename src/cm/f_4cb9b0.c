/* 0x4cb9b0, handler of type 0x48 */

int ai_Unk4cb9b0(G17MSG *m, G17STAT *s)
{
    char *c;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    gm_SetJob(*(unsigned short *)c, m->data[4]);
    gm_ChooseHeadIcon(c);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
