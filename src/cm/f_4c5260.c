/* 0x4c5260, message 0x33 */

void ai_Unk4c5260(int a, int b)
{
    G17MSG m;

    m.type = 0x33;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
