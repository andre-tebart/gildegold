/* 0x4c59f0, message 0x49 with a text */

void ai_Unk4c59f0(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s)
{
    G17MSG m;

    m.data[4] = a;
    G17_I(m, 0xb) = c;
    G17_I(m, 7) = b;
    m.data[0xf] = d;
    m.data[0x11] = f;
    m.type = 0x49;
    m.data[0x10] = e;
    strcpy((char *)&m.data[0x16], s);
    ai_Unk4c39c0(&m);
}
