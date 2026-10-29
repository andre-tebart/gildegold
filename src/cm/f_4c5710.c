/* 0x4c5710, message 0x40: the differences of p (0x80 bytes) to the object k / id of list k */

void cm_SendGoodsDelta(char *p, unsigned char k)
{
    G17MSG m;
    char t[0x80];
    char *o;

    o = gm_FindGoods(k, *(short *)p);
    m.type = 0x40;
    memcpy(t, p, 0x80);
    *(float *)&m.data[0xf] = *(float *)(t + 0x20) - *(float *)(o + 0x20);
    *(float *)&m.data[0x13] = *(float *)(t + 0x24) - *(float *)(o + 0x24);
    *(float *)(t + 0x2c) -= *(float *)(o + 0x2c);
    *(float *)(t + 0x38) -= *(float *)(o + 0x38);
    G17_I(m, 3) = *(int *)(t + 0x10) - *(int *)(o + 0x10);
    G17_S(m, 1) = *(short *)p;
    G17_I(m, 7) = *(int *)(t + 0x14) - *(int *)(o + 0x14);
    G17_I(m, 0xb) = *(int *)(t + 0x18) - *(int *)(o + 0x18);
    m.data[0] = k;
    *(float *)&m.data[0x17] = *(float *)(t + 0x2c);
    *(float *)&m.data[0x1b] = *(float *)(t + 0x38);
    ai_Unk4c39c0(&m);
}
