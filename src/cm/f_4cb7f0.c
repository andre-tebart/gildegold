/* 0x4cb7f0, handler of type 0x42 */

int ai_Unk4cb7f0(G17MSG *m, G17STAT *s)
{
    char *b;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b != 0 && *(int *)(b + 0x61) != 0 && gm_IsHomeType(b) == 0)
        sv_Unk528490(b);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
