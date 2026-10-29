/* 0x521c10, a Ghidra miss (inside the span of 0x521ba0): the progress callback of the scene load (kind, cur, total): the first call creates the bar object 0x623e84 (0x46a4c0, 0x190 wide, below the screen
   centre: 0xfe73c8 * 0.5 + 0xfe73a8 - 205), kind 0 sets it to cur * 200 / total, kind 1 to 200 + cur * 200 / total and at the last step destroys it (0x46e710) and redraws the screen (0x4796c0 .. 0x479480,
   0x522f10 (1, 0), 0x6f22bc = 1); every 10th frame (0x6f2354) the screen is blitted (0x479020 / 0x46aa00 / 0x521ba0 / 0x4790e0 / 0x41e390) (ROUGH) */

#define SV_I(a) (*(int *)(a))

void sv_SceneLoadProgress(int kind, int cur, int total)
{
    int h;

    if (SV_I((unsigned)&full_data_623e84) == -1) {
        SV_I((unsigned)&d2_CutsceneActive) = 1;
        SV_I((unsigned)&full_data_6f2354) = 0;
        h = d2_OpenSlider((int)(*(float *)(unsigned)&d3_ViewW * 0.5f + *(float *)(unsigned)&d3_ViewX - 205.0f), *(short *)(unsigned)&d2_ScreenH - 0x40, 0x190, 0, 0x190, 0x83, 0x42);
        SV_I((unsigned)&full_data_623e84) = h;
        d2_SetObjValue(h, 0, 0x190, 0);
        d2_BlitPart(0, 0, *(int *)(SV_I((unsigned)&d2_BackSurf) + 4), *(int *)(SV_I((unsigned)&d2_BackSurf) + 8), SV_I((unsigned)&d2_ScreenSurf), 0, 0, SV_I((unsigned)&d2_BackSurf));
        if (SV_I((unsigned)&full_data_623e84) == -1)
            goto lend;
    }
    if (kind == 0 || SV_I((unsigned)&full_data_6f2354) % 10 == 0)
        d2_BlitPart(0, 0, *(int *)(SV_I((unsigned)&d2_BackSurf) + 4), *(int *)(SV_I((unsigned)&d2_BackSurf) + 8), SV_I((unsigned)&d2_ScreenSurf), 0, 0, SV_I((unsigned)&d2_BackSurf));
    if (kind == 0) {
        d2_SetObjValue(SV_I((unsigned)&full_data_623e84), 0, 0x190, cur * 200 / total);
    } else if (kind == 1) {
        d2_SetObjValue(SV_I((unsigned)&full_data_623e84), 0, 0x190, cur * 200 / total + 200);
        if (cur >= total - 1) {
            d2_RemoveObj(SV_I((unsigned)&full_data_623e84));
            SV_I((unsigned)&full_data_623e84) = -1;
            SV_I((unsigned)&d2_CutsceneActive) = 0;
            if (SV_I((unsigned)&sv_PanelPic) != -1) {
                d2_ClearSurface(SV_I((unsigned)&d2_ScreenSurf));
                d2_DrawPic(0, 0, SV_I((unsigned)&sv_PanelPic), SV_I((unsigned)&d2_ScreenSurf));
                d2_LockSurface(SV_I((unsigned)&d2_ScreenSurf));
                sv_Unk522f10(1, 0);
                d2_UnlockSurface(SV_I((unsigned)&d2_ScreenSurf));
                d2_BlitPart(0, 0, *(int *)(SV_I((unsigned)&d2_BackSurf) + 4), *(int *)(SV_I((unsigned)&d2_BackSurf) + 8), SV_I((unsigned)&d2_ScreenSurf), 0, 0, SV_I((unsigned)&d2_BackSurf));
            }
            SV_I((unsigned)&ai_FrameWait) = 1;
        }
    }
    if (SV_I((unsigned)&full_data_6f2354) % 10 == 0 && SV_I((unsigned)&full_data_623e84) != -1 && d2_LockSurface(SV_I((unsigned)&d2_ScreenSurf)) != 0) {
        d2_DrawSlider(SV_I((unsigned)&full_data_623e84), SV_I((unsigned)&d2_ScreenSurf));
        sv_DrawPanelPointer();
        d2_UnlockSurface(SV_I((unsigned)&d2_ScreenSurf));
        dd_Flip();
    }
lend:
    SV_I((unsigned)&full_data_6f2354) = SV_I((unsigned)&full_data_6f2354) + 1;
}
