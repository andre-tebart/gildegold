/* 0x4c54e0, message 0x3a */

void ai_Unk4c54e0(int a)
{
    G17MSG m;

    m.type = 0x3a;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
