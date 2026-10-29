/* 0x4c5ec0, message 0x57 */

void ai_Unk4c5ec0(int a)
{
    G17MSG m;

    m.type = 0x57;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
