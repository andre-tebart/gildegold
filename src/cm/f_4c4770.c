/* 0x4c4770, message 0xd */

void ai_Unk4c4770(int a, int b)
{
    G17MSG m;

    m.type = 0xd;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
