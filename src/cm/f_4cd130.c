/* 0x4cd130, handler of type 0x65 */

int ai_Unk4cd130(G17MSG *m, G17STAT *s)
{
    char *q;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    q = cm_PartyById(G17_I(*m, 0));
    if (q == 0)
        return 1;
    *(int *)(q + 0xc) = G17_I(*m, 4);
    if (s != 0)
        s->state = 1;
    return 0;
}
