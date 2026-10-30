/* 0x4ae080, ... kind < 10 and not a human player (6 / 7) */

int ai_IsLivingNonPlayer(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0 && ai_CharsT[id].kind < 10
        && ai_CharsT[id].kind != 6 && ai_CharsT[id].kind != 7)
        return 1;
    return 0;
}
