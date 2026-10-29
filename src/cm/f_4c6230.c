/* 0x4c6230, message 0x66 (two ints of p) */

void ai_Unk4c6230(int *p)
{
    G17MSG m;

    m.type = 0x66;
    G17_I(m, 0) = p[0];
    G17_I(m, 4) = p[1];
    ai_Unk4c39c0(&m);
}
