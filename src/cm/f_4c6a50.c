/* 0x4c6a50, handler of type 1 */

int ai_Unk4c6a50(G17MSG *m, G17STAT *s)
{
    if (s != 0)
        s->state = 1;
    return 0;
}
