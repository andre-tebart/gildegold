/* 0x4c5ef0, message 0x58 */

void ai_Unk4c5ef0(int a)
{
    G17MSG m;

    m.type = 0x58;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
