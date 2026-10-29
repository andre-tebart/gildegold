/* 0x4c60b0, message 0x60 (0x1f bytes of p) */

void ai_Unk4c60b0(void *p)
{
    G17MSG m;

    m.type = 0x60;
    memcpy(m.data, p, 0x1f);
    ai_Unk4c39c0(&m);
}
