/* 0x4c5aa0, message 0x4b (0x80 bytes of p) */

void ai_Unk4c5aa0(void *p)
{
    G17MSG m;

    m.type = 0x4b;
    memcpy(m.data, p, 0x80);
    ai_Unk4c39c0(&m);
}
