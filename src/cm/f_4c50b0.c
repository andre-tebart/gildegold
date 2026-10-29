/* 0x4c50b0, message 0x2d */

void ai_Unk4c50b0(int a, int b, short c)
{
    G17MSG m;

    G17_S(m, 0xc) = c;
    G17_I(m, 8) = b;
    m.type = 0x2d;
    G17_I(m, 4) = a;
    ai_Unk4c39c0(&m);
}
