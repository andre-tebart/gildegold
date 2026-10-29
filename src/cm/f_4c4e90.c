/* 0x4c4e90, message 0x23 */

void ai_Unk4c4e90(int a, int b)
{
    G17MSG m;

    m.type = 0x23;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
