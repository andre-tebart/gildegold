/* 0x4c4f10, message 0x25 */

void ai_Unk4c4f10(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 4) = b;
    m.type = 0x25;
    G17_I(m, 0) = a;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
