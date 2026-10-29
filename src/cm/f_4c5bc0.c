/* 0x4c5bc0, message 0x4e with two texts */

void ai_Unk4c5bc0(const char *s, int a, int b, const char *s2)
{
    G17MSG m;

    m.type = 0x4e;
    strcpy((char *)m.data, s);
    G17_I(m, 0x18) = a;
    G17_I(m, 0x1c) = b;
    if (s2 != 0)
        strcpy((char *)&m.data[0x20], s2);
    else
        m.data[0x20] = 0;
    ai_Unk4c39c0(&m);
}
