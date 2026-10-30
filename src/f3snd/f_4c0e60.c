/* 0x4c0e60, the animal ambience by season (ai_Unk602bd8[season], records birds "Voegel_s", crickets "Grillen_l"
   (hpLocusts / hpLocustsPerm, with a debug log in ai_Unk6f0960), chickens "Gackern_s", horses "Wiehern_s" /
   "Schnauben_s", dogs "Bellt_s"), quieter with the camera height and silent in rain; the bird song of 0x6f0b80 by
   the distance to the player's house; then the atmosphere mp3 of the building the player is in ("Athmos\..")
   @deep B draft, 337 code hunks, two meaning errors fixed: (1) the atmosphere track volume is not ch_Count*100 - the original computes n = ch_Count(d3_CurrentScene, 0) * 9 for the Witshaus (0x16) and * 6 for Handwerk (0x12/0x14/8/0x15/0xe) and Schreibstube (7), caps it with the float 64.0f min (0x5eb808) and keeps it on the FPU stack to the ftol at the play / set-volume call; our source recomputed ch_Count*100.0f at each call; (2) the two d3_Time * 0xd sites are F3SND_TIME (three volatile reads). Read against the original 0x4c0e60..0x4c18a0: the season change, h = (2000 - camera.y - meanHeight) * 0.0005f, the five animal blocks (birds / crickets+hpLocusts / dogs / horses / chickens), the bird song by house distance and the atmosphere branches all correspond in statements, constants and calls. What is left is scheduling / register allocation (337 hunks): stack slots differ ([esp+0x8] vs [esp+0xc]), the min comes out as fcom + jnp where the original has fld 64.0f / fcomp st(1) / jp (the arguments spelled the other way, likely max(64.0f, n)), and the whole body's webs differ. Readability debt: Q1, Q2 (ai_Unk602bd8 the animal table, ai_Unk6f0960 log text, snd_AmbientTrack bird song, ai_Unk6f0b94/98 hpLocusts*, ai_Unk6f0b9c season, ai_Unk6f0ba0 atmosphere track, ai_MarketBuilding player house, ai_PlayerBuilding/2310 building/room - several also declared in cm.h), Q4 9 raw accesses incl. the SND_STOPLOG macro. */

/* stops locust sound s with log text t */
#define SND_STOPLOG(s, t) if ((s) != 0) { sprintf(ai_Unk6f0960, (t), (s)); sb_Stop((s), 0); \
    *(int *)((s) + 0x18) = 0; (s) = 0; }

void ai_Unk4c0e60(void)
{
    unsigned int t;
    int n;
    float h;
    float vol;
    float v[4];
    char *name;
    int k;
    char c;
    G16AMB *a;

    if (d3_Terrain == 0 || snd_AmbientOn.on == 0) {
        t = F3SND_TIME;
        if (ai_Unk6f0b9c != gm_ClockSeason(ai_Time))
            ai_Unk6f0b9c = gm_ClockSeason(ai_Time);
        SND_STOPLOG(ai_Unk6f0b94, "hpLocustsPerm stopped (reason: sound off) %i");
        SND_STOPLOG(ai_Unk6f0b98, "hpLocusts stopped (reason: sound off) %i");
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        a[0].last = t;
        a[1].last = t;
        a[4].last = t;
        a[3].last = t;
        a[2].last = t;
    } else {
        n = 0;
        t = F3SND_TIME;
        if (ai_Unk6f0b9c != gm_ClockSeason(ai_Time)) {
            ai_Unk6f0b9c = gm_ClockSeason(ai_Time);
            if (ai_Unk6f0b98 != 0) {
                sb_Stop(ai_Unk6f0b98, 0);
                *(int *)(ai_Unk6f0b98 + 0x18) = 0;
                ai_Unk6f0b98 = 0;
            }
            if (ai_Unk6f0b94 != 0) {
                sb_Stop(ai_Unk6f0b94, 0);
                *(int *)(ai_Unk6f0b94 + 0x18) = 0;
                ai_Unk6f0b94 = 0;
            }
        }
        h = (2000.0f - *(float *)(ai_Unkfc2dec + 0x50) - d3t_GetMeanHeight(ai_Unkfc2dec + 0x4c)) * 0.0005f;
        if (ai_Unk760618 != 0)
            n = *ai_Unk760618;
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[0].interval) < t - a[0].last && n < 0x15e) {
            a[0].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][0].chance > 1.0f)
                sb_PlaySample(0, "Voegel_s", rand() % ai_Unk602bd8[ai_Unk6f0b9c][0].count + 1, 0x3f,
                    (int)(ai_Unk602bd8[ai_Unk6f0b9c][0].vol * 127.0f), 0, 0);
            if (rand() % 10 > 6)
                sb_PlaySample(0, "Voegel_s", rand() % ai_Unk602bd8[ai_Unk6f0b9c][0].count + 1, 0x3f,
                    (int)(ai_Unk602bd8[ai_Unk6f0b9c][0].vol * 127.0f), 0, 0);
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[1].interval) < t - a[1].last && n < 100) {
            a[1].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][1].chance > 1.0f) {
                if (ai_Unk6f0b98 == 0)
                    k = 0;
                else
                    k = *(int *)(ai_Unk6f0b98 + 0xc);
                if (ai_Unk6f0b98 == 0 || k <= 2) {
                    sprintf(ai_Unk6f0960, "About to trigger hpLocusts! Handle: %i Count: %i", ai_Unk6f0b98, k);
                    ai_Unk6f0b98 = sb_PlaySample(0, "Grillen_l", rand() % ai_Unk602bd8[ai_Unk6f0b9c][1].count + 2,
                        0x3f, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][1].vol * 127.0f), 0, 0);
                    sprintf(ai_Unk6f0960, "hpLocusts triggered!  Handlevalue: %i", ai_Unk6f0b98);
                }
            }
            if (ai_Unk6f0b9c == 1) {
                if (ai_Unk6f0b94 == 0) {
                    ai_Unk6f0b94 = sb_PlaySample(0, "Grillen_l", 0, 0x3f, (int)(0.7f * h * 127.0f) >> 1, 0, 0);
                    sprintf(ai_Unk6f0960, "hpLocustsPerm triggered!  Handlevalue: %i", ai_Unk6f0b94);
                } else {
                    sb_SetVolume(ai_Unk6f0b94, (int)(0.7f * h * 127.0f) >> 1);
                    sprintf(ai_Unk6f0960, "hpLocustsPerm volume adjusted!  Handlevalue: %i", ai_Unk6f0b94);
                }
            } else if (ai_Unk6f0b94 != 0) {
                sprintf(ai_Unk6f0960, "hpLocustsPerm stopped (reason: season) %i", ai_Unk6f0b94);
                sb_Stop(ai_Unk6f0b94, 0);
                ai_Unk6f0b94 = 0;
                *(int *)(ai_Unk6f0b94 + 0x18) = 0;
            }
        } else if (n < 0x65 && ai_Unk6f0b9c != 3) {
            sprintf(ai_Unk6f0960, "Adjusting Volume for hpLocusts: %i %i", ai_Unk6f0b98, ai_Unk6f0b94);
            if (ai_Unk6f0b94 != 0)
                sb_SetVolume(ai_Unk6f0b94, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][1].vol * 127.0f) >> 1);
            if (ai_Unk6f0b98 != 0)
                sb_SetVolume(ai_Unk6f0b98, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][1].vol * 127.0f) >> 1);
        } else {
            SND_STOPLOG(ai_Unk6f0b94, "hpLocustsPerm stopped (reason: numDrops) %i");
            SND_STOPLOG(ai_Unk6f0b98, "hpLocusts stopped (reason: numDrops) %i");
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[4].interval) < t - a[4].last) {
            a[4].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][0].chance > 1.0f)
                sb_PlaySample(0, "Bellt_s", 1, 0x3f, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][4].vol * 127.0f), 0, 0);
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[3].interval) < t - a[3].last) {
            a[3].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][3].chance > 1.0f) {
                name = "Wiehern_s";
                if (rand() % 10 > 4)
                    name = "Schnauben_s";
                sb_PlaySample(0, name, 1, 0x3f, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][3].vol * 127.0f), 0, 0);
            }
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[2].interval) < t - a[2].last) {
            a[2].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][2].chance > 1.0f)
                sb_PlaySample(0, "Gackern_s", rand() % ai_Unk602bd8[ai_Unk6f0b9c][2].count + 1, 0x3f,
                    (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][2].vol * 127.0f), 0, 0);
        }
        if (snd_AmbientTrack != 0 && ai_MarketBuildingV != 0) {
            d3_PointToWorld(ai_MarketBuildingV->object, ai_MarketBuildingV->object + 0x4c, v);
            k = ch_CountAt(0, 0, v, 900.0f) * 0x14;
            if (k > 0x7e)
                k = 0x7f;
            d3snd_SetVolumeI(snd_AmbientTrack, k);
        }
    }
    if (ai_PlayerBuildingB != 0 && snd_AmbientOn.on != 0) {
        c = ai_BTypes[*ai_PlayerBuildingB * 0x24d];
        if (c == 0x16 && sv_PanelShown != 0 && *sv_PanelShown == 0x120) {
            n = ch_Count(d3_CurrentScene, 0) * 9;
            vol = min(n, 64.0f);
            if (ai_Unk6f0ba0 != 0) {
                msx_SetTrackVolume(ai_Unk6f0ba0, (int)vol);
                return;
            }
            ai_Unk6f0ba0 = msx_PlayAmbient("Athmos\\Athmo_Witshaus.mp3", 5);
            return;
        }
        if (c == 0x12 || c == 0x14 || c == 8 || c == 0x15 || c == 0xe) {
            n = ch_Count(d3_CurrentScene, 0) * 6;
            vol = min(n, 64.0f);
            if (ai_Unk6f0ba0 == 0) {
                if (*(int *)(ai_PlayerBuildingB + 0x95) == -1)
                    return;
                ai_Unk6f0ba0 = msx_PlayAmbient("Athmos\\Athmo_Handwerk.mp3", (int)vol);
                return;
            }
            if (*(int *)(ai_PlayerBuildingB + 0x95) == -1) {
                msx_StopAmbient(ai_Unk6f0ba0);
                ai_Unk6f0ba0 = 0;
                return;
            }
            msx_SetTrackVolume(ai_Unk6f0ba0, (int)vol);
            return;
        }
        if (c == 7 && sv_PanelShown != 0 && *sv_PanelShown == 0xf7) {
            n = ch_Count(d3_CurrentScene, 0) * 6;
            vol = min(n, 64.0f);
            if (ai_Unk6f0ba0 == 0) {
                ai_Unk6f0ba0 = msx_PlayAmbient("Athmos\\Athmo_Schreibstube.mp3", (int)vol);
                return;
            }
            msx_SetTrackVolume(ai_Unk6f0ba0, (int)vol);
            return;
        }
    }
    if (ai_Unk6f0ba0 == 0)
        return;
    msx_StopAmbient(ai_Unk6f0ba0);
    ai_Unk6f0ba0 = 0;
}
