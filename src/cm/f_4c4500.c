/* 0x4c4500, message 9: sends one 0x80-byte block of the load buffer (the handler of type 9 appends it);
   returns the send slot of 0x4c39c0, -1 when the ring is full (the ls.c caller tests that) */

int cm_SendLoadBlock(void *p)
{
    G17MSG m;

    m.type = 9;
    memcpy(m.data, p, 0x80);
    return ai_Unk4c39c0(&m);
}
