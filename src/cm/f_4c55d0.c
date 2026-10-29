/* 0x4c55d0, message 0x3d about p at (c, d) */

int cm_SendMessage3d(char *p, unsigned char b, int c, int d)
{
    G17MSG m;
    unsigned short *q;
    char *r;

    m.type = 0x3d;
    if (p == 0)
        return -1;
    q = unk_Unk4fda70(p, c, d);
    G17_I(m, 0) = *(int *)(p + 1);
    if (q == 0) {
        G17_I(m, 9) = -1;
        r = unk_Unk4fd820(0xffff, p);
    } else {
        G17_I(m, 9) = *(int *)&q[2];
        r = *(int *)&q[0xb8] == 0 ? unk_Unk4fd820(q[0], p) : 0;
    }
    if (r != 0)
        G17_I(m, 4) = *(int *)(r + 1);
    else
        G17_I(m, 4) = -1;
    m.data[8] = b;
    m.data[0xd] = (unsigned char)c;
    m.data[0xe] = (unsigned char)d;
    return ai_Unk4c39c0(&m);
}
