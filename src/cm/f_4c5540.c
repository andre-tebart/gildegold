/* 0x4c5540, message 0x39 (b first) */

void cm_SendBoss(int a, int b)
{
    G17MSG m;

    m.type = 0x39;
    G17_I(m, 4) = a;
    G17_I(m, 0) = b;
    ai_Unk4c39c0(&m);
}
