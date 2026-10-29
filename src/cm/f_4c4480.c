/* 0x4c4480, sets the long message (sent as parts with the next message); 0: one is pending or too long */

int ai_Unk4c4480(unsigned short len, void *p)
{
    if (ai_Unka68d30 != 0)
        return 0;
    if (len > 0x3fe)
        return 0;
    *(unsigned short *)ai_Unka68800 = len;
    memcpy(ai_Unka68800 + 2, p, len);
    ai_Unka68d30 = len + 2;
    return 1;
}
