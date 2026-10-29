/* 0x5d7080, the black fade out back to the game (222 B), the twin of 0x5d6fa0: d2_StartFade
   (0, 0, width, height, "BLACK", 0x1e, 1) runs until bit 2, pumping sv_RunFrame (0, 0x24087). When the
   game is not over (ai_GameOver 0x6f259c == 0) the dirty-guard rectangle 0xfb9060 .. 0xfb906c is restored
   from sv_ViewRectLeft .. sv_ViewRectBottom 0x6248a4 .. 0x6248b0, the engine resumes
   (d2_ClearScreen, sv_SetPanelEnabled (1), sv_RedrawScreen, sv_DisableVisibleWidgets (0),
   d2_ReleaseFade, then the black cover d2_StartFade (.., 0x32, 10)); when the game is over only
   d2_ClearScreen / d2_ReleaseFade run and ai_FrameWait 0x6f22bc = 3 keeps the frame for 3 more rounds. */

void ai_ResumeGameScreen(void)
{
    D2FADEVIEW *fade;

    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x1e, 1);
    while ((fade->flags & 4) == 0) {
        sv_RunFrame(0, 0x24087);
    }
    if (ai_GameOver != 0) {
        d2_ClearScreen();
        d2_ReleaseFade(fade);
        ai_FrameWait = 3;
        return 0;
    }
    d2_DirtyRectRight = sv_ViewRectRight;
    d2_DirtyRectLeft = sv_ViewRectLeft;
    d2_DirtyRectTop = sv_ViewRectTop;
    d2_DirtyRectBottom = sv_ViewRectBottom;
    d2_ClearScreen();
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    sv_DisableVisibleWidgets(0);
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x32, 10);
    return 0;
}
