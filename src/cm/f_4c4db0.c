/* 0x4c4db0, sends message 0x20 (the extended system message, handled by cm_ExSysMessage): sub type id and 0x7c bytes of msg */

void cm_SendExSysMessage(unsigned char id, void *msg)
{
    G17MSG m;

    m.type = 0x20;
    m.data[0] = id;
    if (msg != 0)
        memcpy(&m.data[1], msg, 0x7c);
    ai_Unk4c39c0(&m);
}
