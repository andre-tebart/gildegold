/* 0x4c52a0, message 0x34 */

void ai_Unk4c52a0(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x34;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
