/* 0x5dbc30, builds the wind state of the weather (649 B; sv_Unk525510 calls it with the day phase, the game clock's first
   word and the two 24 entry wind rose tables sin / cos of the random wind directions): type (0 .. 3, else the default) is
   the day phase, id the clock word, from / to the sin and cos tables. The state record ai_EffectState gets the clock of
   its last update (year id, hour 6, minute 0; ai_UpdateWind advances from it), a copy of both tables, and the caller's two tables are negated in place (every
   entry * -1). Then for each of the 24 directions three random values are drawn by the phase's constants (base = random * 60 * k + 1, gust = random * 60 * k + c,
   phase = (random + n) * pi or 2 pi): type 0 k 0.25 / 0.3 + 0.2 / 5 * 2 pi, 1 k 0.05 / 0.4 + 0.1 / 3 * pi, 2 k 0.5 / 0.2 + 0.3 /
   5 * 2 pi, 3 k 0.25 / 0.5 / 3 * pi, else 0.5 / 0.5 / 2 * 2 pi. At the end a random angle (0 .. 2 pi) is stored and the
   default effect ai_DefaultEffect gets the vector (the sin table entry 6, 0, the cos table entry 6) and the scale
   |sin(angle) * gust[6]| + base[6]. Void. Reasoned: the wind meaning follows from the caller (the wind rose tables
   of sv_Unk525510) and the sin / cos copies; the arrays base / gust / phase are named from their formulas. */

#include <math.h>

void ai_InitWind(int type, int id, float *from, float *to)
{
    int i;

    ai_EffectState.year = id;
    ai_EffectState.hour = 6;
    ai_EffectState.min = 0;
    memcpy(ai_EffectState.sinTab, from, sizeof(ai_EffectState.sinTab));
    memcpy(ai_EffectState.cosTab, to, sizeof(ai_EffectState.cosTab));
    for (i = 0; i < 24; i++) {
        from[i] = from[i] * -1.0f;
        to[i] = to[i] * -1.0f;
    }
    switch (type) {
    case 0:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.25f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.3f + 0.2f;
            ai_EffectState.phase[i] = (util_RandomF() + 5.0f) * 6.2831855f;
        }
        break;
    case 1:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.05f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.4f + 0.1f;
            ai_EffectState.phase[i] = (util_RandomF() + 3.0f) * 3.1415927f;
        }
        break;
    case 2:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.5f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.2f + 0.3f;
            ai_EffectState.phase[i] = (util_RandomF() + 5.0f) * 6.2831855f;
        }
        break;
    case 3:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.25f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.5f;
            ai_EffectState.phase[i] = (util_RandomF() + 3.0f) * 3.1415927f;
        }
        break;
    default:
        for (i = 0; i < 24; i++) {
            ai_EffectState.base[i] = util_RandomF() * ai_WindGain * 0.5f + 1.0f;
            ai_EffectState.gust[i] = util_RandomF() * ai_WindGain * 0.5f;
            ai_EffectState.phase[i] = (util_RandomF() + 2.0f) * 6.2831855f;
        }
        break;
    }
    ai_EffectState.angle = util_RandomF() * 6.2831855f;
    ai_DefaultEffect.pos[0] = ai_EffectState.sinTab[6];
    ai_DefaultEffect.pos[1] = 0;
    ai_DefaultEffect.pos[2] = ai_EffectState.cosTab[6];
    ai_DefaultEffect.scale = (float)(fabs(sin(ai_EffectState.angle) * ai_EffectState.gust[6]) + ai_EffectState.base[6]);
}
