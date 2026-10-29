/* 0x4c8990, handler of type 0x1f */

int ai_Unk4c8990(G17MSG *m, G17STAT *s)
{
    sv_PopTo(main_GameLoop);
    if (s != 0)
        s->state = 1;
    return 0;
}
