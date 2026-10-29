/* 0x4c5a70, message 0x4a */

void ai_Unk4c5a70(int a)
{
    G17MSG m;

    m.type = 0x4a;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
