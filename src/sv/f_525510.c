/* 0x525510, sets up the weather / sky for the time t (the clock record): the day phase k (0x4fe350), the cloud data 0x760480 / 0x760484 (0x7e4048 / 0x7e4058 by the climate 0x6f1bc5); with a nonzero flag the 0x18
   cloud counters 0x760488 and their heights are rerolled (rain 0x760610: 2 / 1 / 0 at phase 3), the wind rose (sin / cos of random angles, the step +-0.175) filled; then the sky objects 0x45d880 ("Sky_Schoen_01",
   "Sky_Schoen_02", "sky_dunkel_01") and the cloud meshes (0x413b30 / 0x4148f0) are created, every scene with sky data gets 0x525940, and 0x525ad0 runs. ROUGH */

void sv_Unk525510(int t, float flag)
{
    int k, i, mx;
    int idx;
    unsigned *p;
    unsigned short r;
    float ang, step;
    int *q;
    int scene;
    char tmp[12];

    gm_ClockToDate(t, tmp);
    k = gm_ClockSeason(t);
    idx = (ai_OwnPlayer * 0xbd + k) * 4;
    ai_Unk760480 = *(int *)((char *)ai_Unk7e4048 + idx);
    ai_Unk760484 = *(int *)((char *)ai_Unk7e4058 + idx);
    if (flag != 0.0f) {
        if (k == 3) {
            r = util_Random(100);
            ai_Unk760610 = ((int)r < ai_Unk760480 / 2) + 1;
        } else
            ai_Unk760610 = 0;
        for (i = 0; i < 0x18; i++)
            ai_Unk760488[i] = 0;
        p = ai_Unk760488;
        do {
            r = util_Random(100);
            if ((int)r < ai_Unk760480)
                *p = util_Random(ai_Unk760480 * 10) & 0xffff;
            if ((int)*p < 0x2bd) {
                if ((int)*p < 0x1f5) {
                    if ((int)*p < 0xfb)
                        *p = 0;
                    else
                        *p = (*p & 1) + 0x95;
                } else
                    *p = (*p & 1) + 499;
            } else
                *p = (*p & 1) + 999;
            if (ai_Unk760610 == 0 && 0x3e6 < (int)*p)
                p[0x18] = util_Random(3) & 0xffff;
            else
                p[0x18] = 0;
            p++;
        } while ((int)p < (unsigned)((unsigned)ai_Unk760488 + 0x60));
        ang = (float)(util_RandomF() * 6.2831853);
        step = -0.175f;
        if (rand() < 0x4000)
            step = 0.175f;
        for (i = 0; i < 0x18; i++) {
            ai_Unk760550[i] = (float)sin(ang);
            ai_Unk7605b0[i] = (float)cos(ang);
            ang = (float)(util_RandomF() * step + ang);
        }
    }
    ai_InitWind(k, *(int *)ai_Time, ai_Unk760550, ai_Unk7605b0);
    ai_Unk760548 = 0;
    ai_Unk76054c = 0;
    ai_Unk760614 = -1;
    mx = 0;
    for (q = (int *)ai_Unk760488; (int)q < (unsigned)((unsigned)ai_Unk760488 + 0x60); q++)
        if (mx <= *q)
            mx = *q;
    if (ai_Unk760610 == 0) {
        if (ai_Unk76061c != 0) {
            d3t_FreeSnow(ai_Unk76061c);
            ai_Unk76061c = 0;
        }
        if (ai_Unk760618I == 0)
            ai_Unk760618I = d3t_CreateRain(mx);
        d3t_SetRain(ai_Unk760618I, 1, 0, 0);
    } else {
        if (ai_Unk760610 == 1) {
            if (ai_Unk760618I != 0) {
                d3t_DeleteRain(ai_Unk760618I);
                ai_Unk760618I = 0;
            }
        } else if (ai_Unk760618I == 0)
            ai_Unk760618I = d3t_CreateRain(mx);
        if (ai_Unk76061c == 0)
            ai_Unk76061c = d3t_CreateSnow(mx);
        d3t_SetSnow(ai_Unk76061c, 2, 0, 0, 3, 0xff);
        d3t_SetSnow(ai_Unk76061c, 1, 0, 0);
    }
    if (ai_Unk633dd4 == 0)
        ai_Unk633dd4 = d3sky_Create(0xbf733333, 0xbe4ccccd, 0x44160000);
    for (q = (int *)(unsigned)&ai_Unk760620; (int)q < (unsigned)((unsigned)ai_Unk760620 + 0x18); q++) {
        if (*q != 0) {
            d3sky_DeleteLayer(ai_Unk633dd4, *q);
            *q = 0;
        }
    }
    ai_Unk760620 = d3sky_CreateLayer(ai_Unk633dd4, 0, "Sky_Schoen_01", 0xff, *(int *)(unsigned)&full_data_6238e8, 0, *(int *)(unsigned)&full_data_6238ec, *(int *)(unsigned)&full_data_6238f0);
    d3sky_SetLayerSpeed(ai_Unk633dd4, ai_Unk760620, 0);
    ai_Unk760624 = d3sky_CreateLayer(ai_Unk633dd4, 0, "Sky_Schoen_02", 0, *(int *)(unsigned)&full_data_623934, 0, *(int *)(unsigned)&full_data_623938, *(int *)(unsigned)&full_data_62393c);
    d3sky_SetLayerSpeed(ai_Unk633dd4, ai_Unk760624, 0);
    ai_Unk760628 = d3sky_CreateLayer(ai_Unk633dd4, 0, "sky_dunkel_01", 0, *(int *)((unsigned)&ai_Unk623ac0 + 0xd8), 0, *(int *)((unsigned)&ai_Unk623ac0 + 0xdc), *(int *)((unsigned)&ai_Unk623ac0 + 0xe0));
    d3sky_SetLayerSpeed(ai_Unk633dd4, ai_Unk760628, 0);
    scene = d3_CurrentScene;
    i = 0;
    for (q = (int *)&ai_MapRefs; (int)q < ((unsigned)((unsigned)&ai_MapRefs + 0xf7f4) + 0xc); q += 0xf8, i++) {
        if (*q != 0) {
            d3if_SelectScene(i, 1);
            sv_UpdateSkyColors();
        }
    }
    d3if_SelectScene(scene, 1);
    ai_Unk760464 = -100;
    ai_Unk6f2340 = 0;
    sv_Unk525ad0((char *)t);
}
