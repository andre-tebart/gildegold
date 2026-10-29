/* 0x4c5d00, message 0x52 */

void ai_Unk4c5d00(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x52;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
