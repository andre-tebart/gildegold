/* 0x4c5d40, message 0x53 (0x14 bytes of p) */

void ai_Unk4c5d40(void *p)
{
    G17MSG m;

    m.type = 0x53;
    memcpy(m.data, p, 0x14);
    ai_Unk4c39c0(&m);
}
