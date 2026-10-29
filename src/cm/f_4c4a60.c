/* 0x4c4a60, message 0x19 */

void cm_SendSetField(int a, int b, int c, int d, int e)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    m.type = 0x19;
    G17_I(m, 0xc) = d;
    G17_I(m, 0x10) = e;
    ai_Unk4c39c0(&m);
}
