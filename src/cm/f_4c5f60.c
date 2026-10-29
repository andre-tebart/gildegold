/* 0x4c5f60, message 0x5a */

void cm_SendMoneyDelta(int a, int b)
{
    G17MSG m;

    m.type = 0x5a;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
