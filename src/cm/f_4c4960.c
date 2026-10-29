/* 0x4c4960, message 0x14 */

void cm_SendRemoveGoods(int a, short b)
{
    G17MSG m;

    m.type = 0x14;
    G17_I(m, 0) = a;
    G17_S(m, 4) = b;
    ai_Unk4c39c0(&m);
}
