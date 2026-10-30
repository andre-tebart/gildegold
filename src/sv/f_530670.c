/* 0x530670, the thunder / sun glow update of an hour: stops the emitters (0x414590 / 0x415380), a random thunder (sound "DonnerMittel_s", 0x760548 time, 0x76054c length) when the hour's flag 0x7604e8 is set, and
   the light flash 0x417190 / 0x417150 around the hours 11 .. 17 when 0x633444 is 0; ROUGH */

void sv_Unk530670(void)
{
    unsigned short hh;
    int v;

    if (SV_I((unsigned)&ai_Unk760618) != 0)
        d3t_DrawRain(SV_I((unsigned)&ai_Unk760618));
    if (SV_I((unsigned)&ai_Unk76061c) != 0)
        d3t_DrawSnow(SV_I((unsigned)&ai_Unk76061c));
    hh = SV_US((unsigned)&ai_TimeHour);
    if (((int *)(unsigned)&full_data_7604e8)[hh] != 0 && ((short)util_RandomU(300) == 0 || (((int *)(unsigned)&full_data_7604e8)[SV_US((unsigned)&ai_TimeHour)] == 3 && (short)util_RandomU(100) == 0))) {
        SV_I((unsigned)&ai_Unk760548) = SV_I((unsigned)&ai_Unk6333cc);
        SV_I((unsigned)&ai_Unk76054c) = (util_RandomU(8) & 0xffff) + 8;
        sb_PlaySample(SV_I((unsigned)&full_data_6f2408), "DonnerMittel_s", 1, 0x3f, (int)0.0f);
    }
    if (SV_I((unsigned)&d3_CurrentScene) == 0) {
        if (10 < SV_US((unsigned)&ai_TimeHour) && SV_US((unsigned)&ai_TimeHour) < 0x12 && *(int *)((unsigned)&ai_Unk760484 + SV_US((unsigned)&ai_TimeHour) * 4) != 0
            && ((int *)(unsigned)&ai_Unk760488)[SV_US((unsigned)&ai_TimeHour)] == 0 && 0x1e < (int)((unsigned)SV_US(((unsigned)&ai_Time + 0x6)) | ((unsigned)SV_US(((unsigned)&ai_Time + 0x8)) << 16))) {
            hh = (unsigned short)util_RandomU(0x80);
            if (0x60 < hh && SV_I((unsigned)&d3t_SunRays) == 0)
                d3t_CreateSunRays();
            ai_Unk623ecc = SV_US((unsigned)&ai_TimeHour);
        }
        if (ai_Unk623ecc != 0xffffffff && (int)ai_Unk623ecc < (int)SV_US((unsigned)&ai_TimeHour) && 0x1e < (int)((unsigned)SV_US(((unsigned)&ai_Time + 0x6)) | ((unsigned)SV_US(((unsigned)&ai_Time + 0x8)) << 16))) {
            if (SV_I((unsigned)&d3t_SunRays) != 0)
                d3t_FadeOutSunRays(SV_I((unsigned)&d3t_SunRays), 1);
            ai_Unk623ecc = 0xffffffff;
        }
    }
}
