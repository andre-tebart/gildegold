/* 0x4ae080, ... kind < 10 and not a human player (6 / 7) */

int ai_IsLivingNonPlayer(unsigned short id)
{
    if (ai_Chars[id].id != 0xffff && ai_Chars[id].health > 0 && ai_Chars[id].kind < 10
        && ai_Chars[id].kind != 6 && ai_Chars[id].kind != 7)
        return 1;
    return 0;
}
