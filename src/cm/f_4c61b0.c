/* 0x4c61b0, message 100 (0x18 bytes of p) */

void cm_SendCommand(void *p)
{
    G17MSG m;

    m.type = 100;
    memcpy(m.data, p, 0x18);
    ai_Unk4c39c0(&m);
}
