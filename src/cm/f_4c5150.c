/* 0x4c5150, message 0x2f with a name */

void ai_Unk4c5150(int a, int b, int c, const char *s)
{
    G17MSG m;

    m.type = 0x2f;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    strcpy((char *)&m.data[0xc], s);
    ai_Unk4c39c0(&m);
}
