/* 0x4c5460, message 0x36 */

void ai_Unk4c5460(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x36;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
