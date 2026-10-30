/* 0x537980, the loading screen: the form "Misc\Loading_game" (or "Misc\Loading_net" for a network game, 0x6f23f8 & 4), a progress bar (0x46a960), the music / sound hook 0x42e970 (0x44ee40, 7) */

void main_Unk537980(void)
{
    short s;
    char *f;
    int id;

    U_I((unsigned)&d2_CutsceneActive) = 1;
    d2_ClearScreen();
    sv_SetScreenCenter(0x24087);
    s = (short)util_Random(2);
    d2_ShowPicture((-(unsigned)(s != 1) & 3) + 0x78e);
    U_I((unsigned)&ai_Unkfb9060) = -1;
    U_I((unsigned)&ai_Unkfb9068) = -1;
    U_I((unsigned)&ai_Unkfb9064) = -1;
    U_I((unsigned)&ai_Unkfb906c) = -1;
    d3_SetViewport((float)(int)*(short *)(unsigned)&d2_ScreenW, (float)(int)*(short *)(unsigned)&d2_ScreenH, 0.0f, 0.0f, *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH);
    if ((U_I((unsigned)&ai_GameFlags) & 4) == 0)
        f = "Misc\\Loading_game";
    else
        f = "Misc\\Loading_net";
    id = d2_OpenForm(0, 0, f);
    U_I((unsigned)&init_StatusForm) = id;
    d2_MoveObjS(0, *(short *)(unsigned)&d2_ScreenH - *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I(((unsigned)&d2_Views + 0x4) + id * 0x2ac) * 0x3c4) - 0x30, U_I(((unsigned)&ai_Unkf8f13c + 0x250) + U_I(((unsigned)&d2_Views + 0x4) + id * 0x2ac) * 0x3c4));
    d2_PlaceWindow(U_I(((unsigned)&d2_Views + 0x4) + U_I((unsigned)&init_StatusForm) * 0x2ac), 1);
    d2_SetForm(U_I((unsigned)&init_StatusForm), 1);
    d2_WinOpenSlider(0, 0, 0x246, 0, 0x246, 0x83, 0x42, d2_LastWindow);
    d2_DrawAll();
    tb_OpenFunction(d3io_NoopCallback, 7);
}
