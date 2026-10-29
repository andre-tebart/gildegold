/* 0x4c48e0, message 0x12 */

void ai_Unk4c48e0(int a, short b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x12;
    G17_S(m, 4) = b;
    G17_I(m, 6) = c;
    ai_Unk4c39c0(&m);
}
