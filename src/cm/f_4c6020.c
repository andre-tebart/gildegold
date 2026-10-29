/* 0x4c6020, message 0x5d */

void ai_Unk4c6020(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x5d;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
