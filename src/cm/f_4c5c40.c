/* 0x4c5c40, message 0x50 (0x2c bytes of p, the id *ai_Unk6f0d90) */

void ai_Unk4c5c40(int a, void *p)
{
    G17MSG m;

    m.type = 0x50;
    G17_I(m, 0) = a;
    if (ai_Unk6f0d90 == 0)
        G17_I(m, 0x30) = -1;
    else
        G17_I(m, 0x30) = *ai_Unk6f0d90;
    memcpy(&m.data[4], p, 0x2c);
    ai_Unk4c39c0(&m);
}
