/* 0x4c5230, message 0x31 */

void ai_Unk4c5230(int a)
{
    G17MSG m;

    m.type = 0x31;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
