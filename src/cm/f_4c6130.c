/* 0x4c6130, message 0x62 (0x19 bytes of p) */

void ai_Unk4c6130(void *p)
{
    G17MSG m;

    m.type = 0x62;
    memcpy(m.data, p, 0x19);
    ai_Unk4c39c0(&m);
}
