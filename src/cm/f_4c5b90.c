/* 0x4c5b90, message 0x4d */

void ai_Unk4c5b90(int a)
{
    G17MSG m;

    m.type = 0x4d;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
