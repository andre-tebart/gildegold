/* 0x4c46d0, message 0xc (two names of 0x10 bytes) */

void cm_SendMessage0c(int a, int b, short c, unsigned char d, int e, unsigned char f, unsigned char g, const char *s1, const char *s2)
{
    G17MSG m;

    m.type = 0xc;
    G17_I(m, 4) = a;
    G17_I(m, 8) = b;
    G17_S(m, 0xc) = c;
    m.data[0xe] = d;
    G17_I(m, 0xf) = e;
    m.data[0x13] = f;
    m.data[0x14] = g;
    strncpy((char *)&m.data[0x15], s1, 0x10);
    strncpy((char *)&m.data[0x25], s2, 0x10);
    ai_Unk4c39c0(&m);
}
