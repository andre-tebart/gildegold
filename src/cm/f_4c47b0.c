/* 0x4c47b0, message 0xe */

void ai_Unk4c47b0(short a)
{
    G17MSG m;

    m.type = 0xe;
    G17_S(m, 0) = a;
    ai_Unk4c39c0(&m);
}
