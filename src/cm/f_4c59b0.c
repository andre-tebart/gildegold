/* 0x4c59b0, message 0x48 */

void ai_Unk4c59b0(int a, unsigned char b)
{
    G17MSG m;

    m.type = 0x48;
    G17_I(m, 0) = a;
    m.data[4] = b;
    ai_Unk4c39c0(&m);
}
