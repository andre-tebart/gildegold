/* 0x4c5dc0, message 0x55: 0x2c bytes of p, the position / direction of o's model; k 2: also the id of 0x4ce130 */

void ai_Unk4c5dc0(char *o, int k, void *p)
{
    G17MSG m;
    char *v, *r;

    G17_I(m, 0) = *ai_Unk6f0d90;
    m.type = 0x55;
    m.data[4] = (unsigned char)k;
    memcpy(&m.data[0x25], p, 0x2c);
    if (o != 0 && *(int *)(o + 0x184) != 0) {
        v = *(char **)(*(char **)(o + 0x184) + 0x34);
        G17_I(m, 0x21) = *(int *)(o + 0x24);
        G17_I(m, 5) = *(int *)(v + 0x4c);
        G17_I(m, 9) = *(int *)(v + 0x50);
        G17_I(m, 0xd) = *(int *)(v + 0x54);
        G17_I(m, 0x15) = *(int *)(v + 0x84);
        G17_I(m, 0x19) = *(int *)(v + 0x88);
        G17_I(m, 0x1d) = *(int *)(v + 0x8c);
        if (k == 2) {
            r = ai_Unk4ce130(G17_I(m, 0x35));
            if (r != 0)
                G17_I(m, 0x4d) = *(int *)(r + 0x24);
        }
    }
    ai_Unk4c39c0(&m);
}
