/* 0x4ae030, ... and is a player (kind 6 / 7 / 5) */

int ai_IsLivingPlayer(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0
        && (ai_CharsT[id].kind == 6 || ai_CharsT[id].kind == 7 || ai_CharsT[id].kind == 5))
        return 1;
    return 0;
}
