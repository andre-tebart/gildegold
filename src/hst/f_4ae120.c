/* 0x4ae120, ... a plain inhabitant (kind 0): the predicate of the RND_NPC_EINWOHNER token (0x4af8d0) */

int ai_IsLivingInhabitant(unsigned short id)
{
    if (ai_Chars[id].id != 0xffff && ai_Chars[id].health > 0 && ai_Chars[id].kind == 0)
        return 1;
    return 0;
}
