/* 0x4ae0d0, ... a human player (kind 6 / 7) */

int ai_IsLivingHuman(unsigned short id)
{
    if (ai_Chars[id].id != 0xffff && ai_Chars[id].health > 0
        && (ai_Chars[id].kind == 6 || ai_Chars[id].kind == 7))
        return 1;
    return 0;
}
