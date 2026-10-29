/* 0x4c54a0, message 0x38 */

void cm_SendOwnership(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x38;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
