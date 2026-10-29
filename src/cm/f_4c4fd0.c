/* 0x4c4fd0, message 0x2a */

void ai_Unk4c4fd0(int a, int b)
{
    G17MSG m;

    m.type = 0x2a;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
