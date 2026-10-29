/* 0x4c60f0, message 0x61 (0x19 bytes of p) */

void ai_Unk4c60f0(void *p)
{
    G17MSG m;

    m.type = 0x61;
    memcpy(m.data, p, 0x19);
    ai_Unk4c39c0(&m);
}
