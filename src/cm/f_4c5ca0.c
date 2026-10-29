/* 0x4c5ca0, message 0x51, as 0x4c5c40 */

void ai_Unk4c5ca0(int a, void *p)
{
    G17MSG m;

    m.type = 0x51;
    G17_I(m, 0) = a;
    if (ai_Unk6f0d90 == 0)
        G17_I(m, 0x30) = -1;
    else
        G17_I(m, 0x30) = *ai_Unk6f0d90;
    memcpy(&m.data[4], p, 0x2c);
    ai_Unk4c39c0(&m);
}
