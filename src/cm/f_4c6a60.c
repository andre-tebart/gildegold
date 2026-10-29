/* 0x4c6a60, handler of type 4: sets flags of cm_NetFlags */

int ai_Unk4c6a60(G17MSG *m, G17STAT *s)
{
    cm_NetFlags |= m->data[0];
    if (s != 0)
        s->state = 1;
    return 0;
}
