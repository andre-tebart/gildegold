/* 0x4c5580, message 0x3c */

void ai_Unk4c5580(int a, int b, int c, int d)
{
    G17MSG m;

    m.type = 0x3c;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    G17_I(m, 0xc) = d;
    ai_Unk4c39c0(&m);
}
