/* 0x5dc0c0, reads the wind state from the save game (223 B; the save reader 0x5205f0 calls it from version
   0x10050): the fields of ai_EffectState in ai_SaveWind's order with ls_Read. 0 as soon as a read fails,
   else 1. */

int ai_LoadWind(int f)
{
    if (!ls_Read(&ai_EffectState.year, 4, 1, f))
        return 0;
    if (!ls_Read(&ai_EffectState.hour, 4, 1, f))
        return 0;
    if (!ls_Read(&ai_EffectState.min, 4, 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.sinTab, sizeof(ai_EffectState.sinTab), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.cosTab, sizeof(ai_EffectState.cosTab), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.base, sizeof(ai_EffectState.base), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.gust, sizeof(ai_EffectState.gust), 1, f))
        return 0;
    if (!ls_Read(ai_EffectState.phase, sizeof(ai_EffectState.phase), 1, f))
        return 0;
    return ls_Read(&ai_EffectState.angle, 4, 1, f) != 0;
}
