/* 0x4c45c0, message 10 (0x30 bytes of p, or zeros) */

int cm_SendMessage10(unsigned char n, int b, void *p, short d)
{
    G17MSG m;

    m.type = 10;
    if (n == 0)
        return -1;
    *(unsigned short *)&m.data[4] = n;
    G17_I(m, 6) = b;
    G17_S(m, 0xa) = d;
    if (p == 0) {
        memset(&m.data[0x10], 0, 0x30);
        G17_I(m, 0xc) = 0;
    } else {
        memcpy(&m.data[0x10], p, 0x30);
        G17_I(m, 0xc) = 1;
    }
    return ai_Unk4c39c0(&m);
}
