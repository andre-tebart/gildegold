/* 0x4c5e80, message 0x56 (0x28 bytes of p) */

void cm_SendMessage56(void *p)
{
    G17MSG m;

    m.type = 0x56;
    memcpy(m.data, p, 0x28);
    ai_Unk4c39c0(&m);
}
