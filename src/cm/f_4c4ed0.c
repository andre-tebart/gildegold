/* 0x4c4ed0, message 0x24 */

void cm_SendProofExecuted(int a, int b)
{
    G17MSG m;

    m.type = 0x24;
    G17_I(m, 4) = b;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
