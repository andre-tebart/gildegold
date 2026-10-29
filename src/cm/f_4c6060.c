/* 0x4c6060, message 0x5e: o's id (+1) and 0x1c bytes of p */

void ai_Unk4c6060(char *o, void *p)
{
    G17MSG m;

    G17_I(m, 4) = *(int *)(o + 1);
    m.type = 0x5e;
    memcpy(&m.data[8], p, 0x1c);
    ai_Unk4c39c0(&m);
}
