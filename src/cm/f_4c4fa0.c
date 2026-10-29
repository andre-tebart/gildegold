/* 0x4c4fa0, message 0x27: p (0x114 bytes) as the long message */

void ai_Unk4c4fa0(void *p)
{
    G17MSG m;

    m.type = 0x27;
    ai_Unk4c4480(0x114, p);
    ai_Unk4c39c0(&m);
}
