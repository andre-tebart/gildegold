/* 0x4c61f0, message 0x65 */

void ai_Unk4c61f0(int *p, char *q)
{
    G17MSG m;

    m.type = 0x65;
    G17_I(m, 0) = *p;
    G17_I(m, 4) = *(int *)(q + 4);
    ai_Unk4c39c0(&m);
}
