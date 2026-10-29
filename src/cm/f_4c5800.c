/* 0x4c5800, message 0x41 (0x3a bytes of p) */

void cm_SendMessage41(void *p)
{
    G17MSG m;

    m.type = 0x41;
    memcpy(m.data, p, 0x3a);
    ai_Unk4c39c0(&m);
}
