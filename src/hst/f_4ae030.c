/* 0x4ae030, ... and is a player (kind 6 / 7 / 5) */

int ai_IsLivingPlayer(unsigned short id)
{
    if (ai_Chars[id].id != 0xffff && ai_Chars[id].health > 0
        && (ai_Chars[id].kind == 6 || ai_Chars[id].kind == 7 || ai_Chars[id].kind == 5))
        return 1;
    return 0;
}
