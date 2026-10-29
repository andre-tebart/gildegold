/* 0x4c4e00, message 0x21 */

void cm_SendKillCharacter(int a, int b)
{
    G17MSG m;

    m.type = 0x21;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
