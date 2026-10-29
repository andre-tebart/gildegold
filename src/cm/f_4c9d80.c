/* 0x4c9d80, handler of type 0x34 */

int ai_Unk4c9d80(G17MSG *m, G17STAT *s)
{
    char *o;

    o = gm_PlaceOnStage(G17_I(*m, 0), G17_I(*m, 4), G17_I(*m, 8), 0, 0);
    if (o == 0)
        return 1;
    if (s != 0) {
        s->state = 2;
        s->unk01 = 7;
        s->value = (int)o;
    }
    return 0;
}
