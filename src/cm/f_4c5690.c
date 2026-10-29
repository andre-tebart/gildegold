/* 0x4c5690, message 0x3e with a text */

void ai_Unk4c5690(int a, const char *s)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x3e;
    strcpy((char *)&m.data[4], s);
    ai_Unk4c39c0(&m);
}
