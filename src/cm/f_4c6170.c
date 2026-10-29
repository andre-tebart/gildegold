/* 0x4c6170, message 99 (0x24 bytes of p) */

void cm_SendCreateAlchemist(void *p)
{
    G17MSG m;

    m.type = 99;
    memcpy(m.data, p, 0x24);
    ai_Unk4c39c0(&m);
}
