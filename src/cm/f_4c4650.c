/* 0x4c4650, message 0xb */

void cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h)
{
    G17MSG m;

    m.type = 0xb;
    m.data[4] = a;
    G17_I(m, 5) = b;
    G17_I(m, 9) = c;
    *(unsigned short *)&m.data[0xd] = d;
    G17_I(m, 0xf) = e;
    m.data[0x13] = f;
    m.data[0x14] = g;
    m.data[0x15] = h;
    ai_Unk4c39c0(&m);
}
