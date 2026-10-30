/* 0x4ae0d0, ... a human player (kind 6 / 7) */

int ai_IsLivingHuman(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0
        && (ai_CharsT[id].kind == 6 || ai_CharsT[id].kind == 7))
        return 1;
    return 0;
}
