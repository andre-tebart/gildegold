/* 0x4c62a0, message 0x68: the route points pts (0x40 bytes each, next at +0x3c) of o as the long message */

void ai_Unk4c62a0(char *o, char *pts)
{
    G17MSG m;
    char buf[0x200];
    char *d, *next;
    int i;

    G17_I(m, 4) = rt_PointCount(pts);
    G17_I(m, 0) = *(int *)(o + 2);
    m.type = 0x68;
    for (i = G17_I(m, 4), d = buf; i > 0; i--) {
        next = *(char **)(pts + 0x3c);
        memcpy(d, pts, 0x40);
        d += 0x40;
        pts = next;
    }
    ai_Unk4c4480(G17_I(m, 4) << 6, buf);
    ai_Unk4c39c0(&m);
}
