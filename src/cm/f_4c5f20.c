/* 0x4c5f20, message 0x59 (0x48 bytes of p) */

void ai_Unk4c5f20(void *p)
{
    G17MSG m;

    m.type = 0x59;
    memcpy(m.data, p, 0x48);
    ai_Unk4c39c0(&m);
}
