/* 0x4c4880, message 0x11 */

void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f)
{
    G17MSG m;

    m.type = 0x11;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_S(m, 8) = c;
    m.data[0xe] = e;
    G17_I(m, 0xf) = d;
    G17_I(m, 0x13) = f;
    ai_Unk4c39c0(&m);
}
