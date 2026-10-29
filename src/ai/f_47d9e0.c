/* 0x47d9e0, the AI module's init at the start of a game: resets the character update cycle (start index, last
   update time at day 0 06:00, count carry), loads the action table (returns 0 when that fails) and seeds the two
   weapon lists */

int ai_Init(void)
{
    ai_UpdateStart = 0;
    ai_LastUpdateTime.year = 0;
    gm_SetTime(&ai_LastUpdateTime, 6, 0, 0);
    ai_CountCarry = 0;
    if (!ai_LoadActionTable())
        return 0;
    ai_WeaponList[5] = 0;
    ai_WeaponList2[4] = 0;
    ai_WeaponList[1] = 0x154;
    ai_WeaponList[0] = 0x156;
    ai_WeaponList[2] = 0x158;
    ai_WeaponList[3] = 0x172;
    ai_WeaponList[4] = 0x16e;
    ai_WeaponList2[0] = 0x174;
    ai_WeaponList2[1] = 0x176;
    ai_WeaponList2[2] = 0x15e;
    ai_WeaponList2[3] = 0x160;
    return 1;
}
