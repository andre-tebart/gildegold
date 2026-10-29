/* 0x4c5510, message 0x3b */

void ai_Unk4c5510(int a)
{
    G17MSG m;

    m.type = 0x3b;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
