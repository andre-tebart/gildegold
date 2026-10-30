/* 0x4c0470, the rain sounds by the rain level *ai_Unk760618 (near / far loops louder the lower / higher the camera
   is above the ground, drops "Tropfen_s"), then now and then a gust ("Boehe_*", chance and interval by season from
   the table 0x602ba8); everything stops without a scene, with sound off or when the season changed
   @deep B draft, 571 code hunks, one meaning error fixed (the gust volume: the original loads snd_Gusts[season].vol and multiplies by 127.0f, our source invented rand()*3.0518509e-05f*100.0f; G16SEASON.unk04 is a float vol, now typed). Read the rest of the original against the source: the four rain-level branches (n >= 999 / 499 / 149 / else), every volume formula and its operand order, the Tropfen_s delays 1000 / 0xa28 / 3000 / 4000 and the +0x18 repeat-delay stores, the stop lists of each branch, the gust chance test and the k = rand()%3 / %2 selection all correspond. What is left is scheduling / register allocation: the original keeps w = n*8 in ebp and f / n in [esp+0x10] / [esp+0x18] inside a 0xc frame; ours builds a 0x10 frame without ebp and allocates different webs over the whole body. Readability debt: Q2 ai_Unk760618 (the rain level, declared int in cm.h/sv.c and int * here - a cross-header conflict in lint_baseline) and ai_Unkfc2dec (0xfc2dec is d3_Camera in d3.h, 127 references in 6 files, conflicting char *; making it d3_Camera needs the d3.h type in this TU); Q4 the camera position (0x4c/0x50) and the SBSAMPLE.delay (+0x18) raw accesses need typed views for the same reason. */

/* stops ambient sound s (a macro: the original repeats this block for each one) */
#define SND_STOP(s) if ((s) != 0) { sb_Stop((s), 0); (s) = 0; }

/* plays ambient sound s (name n, volume v; a, b the last two arguments) or sets its volume */
#define SND_SET(s, n, v, a, b) if ((s) == 0) (s) = sb_PlaySampleI(snd_Bank, (n), 0, 0x3f, (v), (a), (b)); \
    else sb_SetVolumeI((s), (v))

void snd_UpdateRain(void)
{
    float f;
    int n;
    int w;
    int v;
    int v2;
    int k;

    if (d3_Terrain == 0 || snd_AmbientOn.on == 0 || snd_Season != gm_ClockSeason(ai_Time)) {
        snd_Season = gm_ClockSeason(ai_Time);
        SND_STOP(snd_RainHeavyFar);
        SND_STOP(snd_RainHeavyNear);
        SND_STOP(snd_RainMediumFar);
        SND_STOP(snd_RainMediumNear);
        SND_STOP(snd_RainLightFar);
        SND_STOP(snd_RainLightNear);
        SND_STOP(snd_RainDrops);
        return;
    }
    w = 0;
    f = *(float *)(ai_Unkfc2dec + 0x50) - d3t_GetMeanHeight(ai_Unkfc2dec + 0x4c);
    memcpy(snd_TimeCopy, ai_Time, 14);
    if (ai_Unk760618 == 0 || (n = *ai_Unk760618) == 0) {
        SND_STOP(snd_RainHeavyFar);
        SND_STOP(snd_RainHeavyNear);
        SND_STOP(snd_RainMediumFar);
        SND_STOP(snd_RainMediumNear);
        SND_STOP(snd_RainLightFar);
        SND_STOP(snd_RainLightNear);
        SND_STOP(snd_RainDrops);
    } else {
        w = n * 8;
        if (n >= 999) {
            v = (int)((2600.0f - f) * 88.9f * 0.000384615385f);
            SND_SET(snd_RainHeavyNear, "StarkNah_l_01", v, 0, 0);
            SND_SET(snd_RainHeavyFar, "StarkFern_l_01", (int)(f * 88.9f * 0.000384615385f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 0, 0x3f, v, 0, 1000);
            else
                sb_SetVolumeI(snd_RainDrops, v);
            SND_STOP(snd_RainMediumNear);
            SND_STOP(snd_RainLightNear);
            SND_STOP(snd_RainMediumFar);
            SND_STOP(snd_RainLightFar);
        } else if (n >= 499) {
            k = n - 499;
            v = (int)((2600.0f - f) * k * 88.9f * 7.6923078e-07f);
            SND_SET(snd_RainHeavyNear, "StarkNah_l_01", v, 0, 0);
            SND_SET(snd_RainHeavyFar, "StarkFern_l_01", (int)(k * f * 88.9f * 7.6923078e-07f), 0, 0);
            v2 = (int)((2600.0f - f) * (499 - k) * 88.9f * 7.6923078e-07f);
            SND_SET(snd_RainMediumNear, "MittelNah_l_01", v2, 0, 0);
            SND_SET(snd_RainMediumFar, "MittelFern_l_01", (int)((499 - k) * f * 88.9f * 7.6923078e-07f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 0, 0x3f, v, 0, 0xa28);
            else {
                *(int *)(snd_RainDrops + 0x18) = 2000;
                sb_SetVolumeI(snd_RainDrops, v);
            }
            SND_STOP(snd_RainLightFar);
            SND_STOP(snd_RainLightNear);
        } else if (n >= 0x95) {
            k = n - 0x95;
            v = (int)((2600.0f - f) * k * 88.9f * 1.0989011e-06f);
            SND_SET(snd_RainMediumNear, "MittelNah_l_01", v, 0, 0);
            SND_SET(snd_RainMediumFar, "MittelFern_l_01", (int)(k * f * 88.9f * 1.0989011e-06f), 0, 0);
            v2 = (int)((2600.0f - f) * (350 - k) * 88.9f * 1.0989011e-06f);
            SND_SET(snd_RainLightNear, "LeichtNah_l_01", v2, 0, 0);
            SND_SET(snd_RainLightFar, "LeichtFern_l_01", (int)((350 - k) * f * 88.9f * 1.0989011e-06f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 1, 0x3f, v, 0, 3000);
            else {
                *(int *)(snd_RainDrops + 0x18) = 3000;
                sb_SetVolumeI(snd_RainDrops, v);
            }
            SND_STOP(snd_RainHeavyFar);
            SND_STOP(snd_RainHeavyNear);
        } else {
            v = (int)((2600.0f - f) * n * 88.9f * 2.5813113e-06f);
            SND_SET(snd_RainLightNear, "LeichtNah_l_01", v, 0, 0);
            SND_SET(snd_RainLightFar, "LeichtFern_l_01", (int)(n * f * 88.9f * 2.5813113e-06f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 0, 0x3f, v, 0, 4000);
            else {
                *(int *)(snd_RainDrops + 0x18) = 4000;
                sb_SetVolumeI(snd_RainDrops, v);
            }
            SND_STOP(snd_RainHeavyFar);
            SND_STOP(snd_RainHeavyNear);
            SND_STOP(snd_RainMediumFar);
            SND_STOP(snd_RainMediumNear);
        }
    }
    if (d3_Terrain != 0 && snd_AmbientOn.on != 0
        && snd_Gusts[snd_Season].interval <= (unsigned int)(w + (F3SND_TIME - snd_LastGust))) {
        snd_LastGust = F3SND_TIME;
        if (rand() * 3.0518509e-05f + snd_Gusts[snd_Season].chance > 1.0f) {
            k = 0;
            if (w >= 0x28b)
                k = rand() % 3;
            else if (w > 0xfa)
                k = rand() % 2;
            sb_PlaySampleI(0, snd_GustNames[k], 1, 0x3f, (int)(snd_Gusts[snd_Season].vol * 127.0f), 0, 0);
        }
    }
}
