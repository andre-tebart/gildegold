/* 0x537be0, closes the loading screen: fades to black (0x4767d0 "BLACK" 0x1e), runs the frame loop until the fade ends, frees the form 0x75df00 and clears the busy flag 0x6ed9d4 */

void main_Unk537be0(void)
{
    unsigned char *fade;
    unsigned mask;

    mask = 0x24087;
    if (U_I((unsigned)&ai_Unk1460ae0) == -1)
        mask = 0x24086;
    tb_CloseFunction(d3io_NoopCallback);
    fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x1e, 1);
    while ((*fade & 4) == 0)
        sv_RunFrame(0, mask);
    d2_CloseView(U_I((unsigned)&init_StatusForm));
    ((void (*)(void))d3io_NoopCallback)();  /* called with no argument (no push at the call) */
    d2_ClearSurface(d2_ScreenSurf);
    U_I((unsigned)&d2_CutsceneActive) = 0;
    d2_ReleaseFade(fade);
}
