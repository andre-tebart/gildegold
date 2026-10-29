/* 0x4c6260, message 0x67 (0x80 bytes of p) */

void ai_Unk4c6260(void *p)
{
    G17MSG m;

    m.type = 0x67;
    memcpy(m.data, p, 0x80);
    ai_Unk4c39c0(&m);
}
