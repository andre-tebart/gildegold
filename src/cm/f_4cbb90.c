/* 0x4cbb90, handler of type 0x4a */

int ai_Unk4cbb90(G17MSG *m, G17STAT *s)
{
    char *t;

    t = gm_FindObjectInst(G17_I(*m, 0));
    if (t == 0)
        return 1;
    gm_Unk5034b0(t);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
