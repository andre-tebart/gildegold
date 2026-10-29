/* 0x4c5840, message 0x42 */

void ai_Unk4c5840(int a)
{
    G17MSG m;

    m.type = 0x42;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
