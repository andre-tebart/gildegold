/* 0x4c4e40, message 0x22: 0x2d bytes of p, the id ai_Unk6f1b88, 1 and a random number */

void ai_Unk4c4e40(void *p)
{
    G17MSG m;

    m.type = 0x22;
    memcpy(m.data, p, 0x2d);
    G17_I(m, 0) = ai_Unk6f1b88;
    G17_I(m, 0x25) = 1;
    G17_I(m, 0x29) = rand();
    ai_Unk4c39c0(&m);
}
