/* 0x4c6ad0, handler of type 9: appends 0x80 bytes to the load buffer */

int ai_Unk4c6ad0(G17MSG *m, G17STAT *s)
{
    memcpy(ai_Unkf31140 + ai_Unkf81220, m->data, 0x80);
    ai_Unkf81220 += 0x80;
    if (s != 0)
        s->state = 1;
    return 0;
}
