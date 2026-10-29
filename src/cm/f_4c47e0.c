/* 0x4c47e0, message 0xf */

void cm_SendMoveGoods(int a, int b, unsigned char c, int d)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    m.type = 0xf;
    m.data[0xc] = c;
    G17_I(m, 0xd) = d;
    ai_Unk4c39c0(&m);
}
