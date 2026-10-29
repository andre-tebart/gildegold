/* 0x4c5ae0, "cm_RequestCreateGeb": message 0x4c, a building of kind k at pos */

int cm_RequestCreateGeb(int a, int k, float *pos, int *v)
{
    G17MSG m;
    char buf[0x100];

    m.type = 0x4c;
    if (k == 0)
        return -1;
    m.data[8] = (unsigned char)k;
    G17_I(m, 0) = a;
    *(float *)&m.data[9] = pos[0];
    *(float *)&m.data[0xd] = pos[1];
    *(float *)&m.data[0x11] = pos[2];
    *(float *)&m.data[0x15] = pos[3];
    G17_I(m, 0x19) = v[0];
    G17_I(m, 0x1d) = v[1];
    G17_I(m, 0x21) = v[2];
    sprintf(buf, "cm_RequestCreateGeb(): pos %f   %f   %f", pos[0], pos[1], pos[2]);
    return ai_Unk4c39c0(&m);
}
