/* 0x4adff0, character id exists, lives and has kind < 10 */

int ai_IsLivingCharacter(unsigned short id)
{
    if (ai_Chars[id].id != 0xffff && ai_Chars[id].health > 0 && ai_Chars[id].kind < 10)
        return 1;
    return 0;
}
