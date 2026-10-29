/* 0x4c6320, message 0x69: o's id (+2) */

void ai_Unk4c6320(char *o)
{
    G17MSG m;

    m.type = 0x69;
    G17_I(m, 0) = *(int *)(o + 2);
    ai_Unk4c39c0(&m);
}
