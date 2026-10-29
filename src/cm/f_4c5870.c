/* 0x4c5870, message 0x43 */

void ai_Unk4c5870(int a)
{
    G17MSG m;

    m.type = 0x43;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
