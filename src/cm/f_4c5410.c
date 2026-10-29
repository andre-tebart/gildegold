/* 0x4c5410, message 0x37 with a name */

void ai_Unk4c5410(int a, const char *s, unsigned char c)
{
    G17MSG m;

    m.type = 0x37;
    G17_I(m, 0) = a;
    m.data[0x34] = c;
    strncpy((char *)&m.data[4], s, 0x2f);
    ai_Unk4c39c0(&m);
}
