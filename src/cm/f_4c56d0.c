/* 0x4c56d0, message 0x3f: k and 0x80 bytes of p */

void ai_Unk4c56d0(void *p, unsigned char k)
{
    G17MSG m;

    m.type = 0x3f;
    m.data[0] = k;
    memcpy(&m.data[1], p, 0x80);
    ai_Unk4c39c0(&m);
}
