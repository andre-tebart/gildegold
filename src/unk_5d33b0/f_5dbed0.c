/* 0x5dbed0, advances the wind (235 B; sv_RunFrame calls it every frame): the hours since the state's last
   update (gm_MinutesBetween of the stored clock and ai_Time, / 60); when not negative, the angle of
   ai_EffectState grows by hours * phase[hour], the default effect ai_DefaultEffect gets the hour's wind
   vector (sin, 0, cos) and the scale (sin(angle) + 1) * gust[hour] / 2 + base[hour], and the clock is
   stored as the last update. */

void ai_UpdateWind(void)
{
    GAMECLOCK last;
    GAMECLOCK *now = (GAMECLOCK *)ai_Time;
    float hours;
    int hour;

    last.year = ai_EffectState.year;
    last.hour = (unsigned short)ai_EffectState.hour;
    last.min = ai_EffectState.min;
    last.xy = 0;
    hours = gm_MinutesBetween((struct AITIME *)&last, (struct AITIME *)ai_Time) * (1.0f / 60.0f);
    if (!(hours < 0.0f)) {
        hour = now->hour;
        ai_DefaultEffect.pos[1] = 0;
        ai_EffectState.angle += hours * ai_EffectState.phase[hour];
        ai_DefaultEffect.pos[0] = ai_EffectState.sinTab[hour];
        ai_DefaultEffect.pos[2] = ai_EffectState.cosTab[hour];
        ai_DefaultEffect.scale = ((float)sin(ai_EffectState.angle) + 1.0f) * ai_EffectState.gust[hour] * 0.5f
            + ai_EffectState.base[hour];
        ai_EffectState.year = now->year;
        ai_EffectState.hour = hour;
        ai_EffectState.min = now->min;
    }
}
