/* 0x513740, after loading a town: the "now" of the game clock (0x4fe350 of the clock 0x770ae0, kept as the season / day part 0..3 in 0x6f1c24), the camera of the player's character (0x506430, 0x43b790 / 0x525dd0 / 0x43b860, the default 0.33 when it has none), the characters without a house (0x2c = -1) that are not in the shown scene are hidden (0x402540 / 0x402480 / 0x4fb5a0), the scene callbacks 0x512de0 (0xe0) / 0x512e40 (0xc0), the scene's own hook (+0x210 of its parent), the frame limits (0x5128d0 / 0x513590) and the light factor 0x5fbd04 = 1 - 0.01 * the detail byte 0x6f35cc; then 0x525ad0 and the scene switch 0x447860 (0 when 0x6339db is set, else 1). ROUGH */


void unk_Unk513740(void)
{
    int phase, c;
    int *p;
    char *e;

    phase = gm_ClockSeason(ai_Time);
    if (d3_CurrentScene == 0) {
        c = (int)gm_PlayerRecord(ai_Chars[(unsigned short)ai_PlayerId]);
        if (c == 0 || *(int *)(c + 0x88) == -0x40800000) {
            sv_PlaceCamera(0x3ea8f5c3);
        } else {
            d3_SetObjectPos(ai_Unkfc2dec, c + 0x8c);
            sv_PlaceCamera(*(int *)(c + 0x88));
            *(int *)(ai_Unkfc2dec + 0x88) = *(int *)(c + 0xa0);
            d3_SetObjectRot(ai_Unkfc2dec, ai_Unkfc2dec + 0x84);
        }
        for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < (int)((unsigned)&ai_Unk770cec + 0x65418); p += 0x87) {
            if (*p != 0) {
                if (d3t_SceneIndex(*(int *)(*p + 0x88)) == 0 && *(int *)(*p + 0x2c) == -1) {
                    int o = *p;

                    if (*(int *)(*(int *)(o + 0x34) + 0x1cc) == 0
                        || (*(int *)(o + 0x22c) != 0 && *(int *)(*(int *)(o + 0x22c) + 0x1cc) == 0))
                        ch_Unk402540(o);
                    ch_SetVisible(*p, 1);
                    gm_SelectModelVariant(p - 0x61);
                }
            }
        }
        if (phase == 1)
            init_TextureSet = 1;
        else if (phase == 0)
            init_TextureSet = 0;
        else if (phase == 2)
            init_TextureSet = 2;
        else if (phase == 3)
            init_TextureSet = 3;
        d3_ForEachObject2(d3_DefaultScene, 0, 0xe0, init_PlantTextureSetHook);
        d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_TextureSetHook);
        if (d3_DefaultScene != 0) {
            int hook = *(int *)((char *)d3_DefaultScene + 0x3c8);

            if (hook != 0 && *(void (**)())(hook + 0x210) != 0)
                (*(void (**)())(hook + 0x210))(d3_DefaultScene, 4, hook + 0x214, 0);
        }
        init_SetSeasonTextures();
        init_SetDetailLevel((int)ai_Unk633738, 0);
        d3_FogByDistance = 1;
        evt_NoGroup = 0;
        d3o_FogScale = 1.0f - (float)(ai_GfxFog & 0xff) * 0.01f;
        sv_Unk525ad0(ai_Time);
        if (ai_Unk6339db != 0) {
            d3if_Prerender(0);
            return;
        }
        d3if_Prerender(1);
    }
}
