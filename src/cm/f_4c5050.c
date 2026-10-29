/* 0x4c5050, message 0x2c */

void cm_SetPlant(int a, unsigned char b, unsigned char c, short d, unsigned char e, int f)
{
    G17MSG m;

    m.type = 0x2c;
    G17_I(m, 4) = a;
    m.data[8] = b;
    m.data[9] = c;
    G17_S(m, 0xa) = d;
    m.data[0xc] = e;
    G17_I(m, 0xd) = f;
    ai_Unk4c39c0(&m);
}
