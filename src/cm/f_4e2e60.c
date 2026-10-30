/* 0x4e2e60, loads the scene file name into a free scene ("scenes/*%s"), selects it, sets the detail flags of its +0x1c70 byte by the
   setting 0x6f35ca (bits 3 / 4), freezes the screen (0x6ed924 / 0x6ed92c) when the window flag +0x26 & 0x20 of 0x6f0d90 is set, runs the
   scene's callback (+0x3c8 -> +0x210) once, counts the scene depth 0x6f0d9c and fades from black ("BLACK", 0x5a ticks). Not in the replay mode */

void ai_Unk4e2e60(char *name)
{
    char path[0x80];
    int idx, sh, t, k;
    int (*cb)();

    if (ai_Unk6f0d8c != 0)
        return;
    idx = d3t_FindFreeScene();
    d3if_SelectScene(idx, 0);
    sprintf(path, "scenes/*%s", name);
    d3_LoadScene(path, 0, 0);
    d3if_SelectScene(0, 1);
    d3if_SelectScene(idx, 0);
    if (ai_Unk633738 != 0) {
        if (ai_Unk6f35ca == 0) {
            *(unsigned char *)((char *)ai_Unk633738 + 0x1c70) = (*(unsigned char *)((char *)ai_Unk633738 + 0x1c70) & 0xeb) | 0x8;
            sh = 2;
        } else {
            *(unsigned char *)((char *)ai_Unk633738 + 0x1c70) = (*(unsigned char *)((char *)ai_Unk633738 + 0x1c70) & 0xe7) | 0x4;
            sh = 1;
        }
        d3fl_SetDetail(0x40);
        d3fl_SetShift((struct D3FLMAP *)ai_Unk633738, sh >> 1, 1);
    }
    if (((char *)ai_Unk6f0d90)[0x26] & 0x20) {
        d2_ClearSurface(d2_ScreenSurf);
        G27_FRAME(d2_ScreenSurf, ai_Unk6ed92c);
        sv_RunFrame(0, 0x86);
        G27_FRAME(ai_Unk6ed92c, d2_ScreenSurf);
    }
    if (gm_ClockSeason(ai_Time) == 3)
        sv_UpdateSkyColors();
    k = ai_Unk6f0d9c + 1;
    evt_NoGroup = -2;
    ai_Unk6f0d9c = k;
    if (ai_Unk5fb5b0 != 0) {
        t = *(int *)(ai_Unk5fb5b0 + 0x3c8);
        if (t != 0) {
            cb = *(int (**)())(t + 0x210);
            if (cb != 0)
                cb(ai_Unk5fb5b0, 4, t + 0x214, 0);
        }
    }
    ai_Unk60d36c = ai_Unk5f8438;
    d3if_Prerender(1);
    if (ai_Unk6f0da0 != 0) {
        d2_ReleaseFade(ai_Unk6f0da0);
        ai_Unk6f0da0 = 0;
    }
    d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
}
