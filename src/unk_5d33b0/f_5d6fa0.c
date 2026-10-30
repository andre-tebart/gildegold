/* 0x5d6fa0, the black fade in of a screen change (259 B): d2_StartFade (0, 0, screen width
   0xfb9072, height 0xfb9070, "BLACK" 0x5fea38, 0x1e, 1) runs until the fade record's flags byte has bit 2,
   pumping sv_RunFrame with the frame flags 0x760638 & ~0x2000 | 0x100000. Then the dirty-guard rectangle
   0xfb9060 .. 0xfb906c takes the current screen size (right = top = 0, so every d2_AddDirty test hits and
   the game screen redraws fully), 0x760638 = 0x24087, and the engine is reset
   (sv_DisableVisibleWidgets (1), d2_ClearScreen, d2_ShowPicture (0x794), d2_ReleaseFade (fade),
   sv_SetPanelEnabled (0)) and a second black fade (0x32, 10) is started as the new screen's black cover. */

void ai_LeaveGameScreen(void)
{
    D2FADEVIEW *fade;
    int flags;
    short w, h;

    flags = (sv_FrameMaskU & ~0x2000) | 0x100000;
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x1e, 1);
    while ((fade->flags & 4) == 0) {
        sv_RunFrame(0, flags);
    }
    d2_DirtyRectRight = 0;
    w = d2_ScreenW;
    d2_DirtyRectLeft = w;
    d2_DirtyRectTop = 0;
    h = d2_ScreenH;
    d2_DirtyRectBottom = h;
    sv_FrameMaskU = 0x24087;
    sv_DisableVisibleWidgets(1);
    d2_ClearScreen();
    d2_ShowPicture(0x794);
    d2_ReleaseFade(fade);
    sv_SetPanelEnabled(0);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x32, 10);
}
