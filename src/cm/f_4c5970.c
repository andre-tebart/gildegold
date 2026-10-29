/* 0x4c5970, message 0x47 */

void ai_Unk4c5970(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x47;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
