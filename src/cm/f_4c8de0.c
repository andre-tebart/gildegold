/* 0x4c8de0, handler of type 0x21: a character (data[0]) is removed / re-set */

int ai_Unk4c8de0(G17MSG *m, G17STAT *s)
{
    int ok = 0;
    char *c;

    if (s != 0)
        s->state = 2;
    c = gm_FindChar(G17_I(*m, 0));
    if (c != 0) {
        ok = 1;
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        if (*(char **)(c + 0x184) != 0 && *(int *)(*(char **)(c + 0x184) + 0x28) != -1)
            evt_Stop(evt_FindById(*(int *)(*(char **)(c + 0x184) + 0x28)));
        unk_Unk503c60(*(unsigned short *)c, G17_I(*m, 4));
    }
    if (s != 0 && ok)
        s->state = 1;
    return 0;
}
