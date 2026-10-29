/* 0x5d7ba0, the credits screen (649 B), opened by the credits button of the main menu (main.c
   0x533850, "ZumGutenEnd.mp3"): a black cover fades in (d2_StartFade 0x4767d0 (0, 0, width, height,
   "BLACK", 0x3c, 1), its flags byte bit 2 = done, pumped with sv_RunFrame 0x5309a0 (0, 0x24087)),
   then the screen switches (d2_ReleaseFade 0x476860, d2_ClearScreen 0x46e990, d2_ShowPicture 0x46cfd0
   (0x7ad), a new black cover (.., 0x3c, 10)) and the credits scroll in a 600-wide window at (100, 0)
   (d2_CreateWindow 0x472390, d2_PlaceWindowScaled 0x475730 (bar, 1)): the text (io_PrintText 0x515960
   (0x2004)) starts one screen height above the window (scrollY = -height) and the window is pumped
   (sv_RunFrame (this, 0x24087)) until the pump returns 0, a key / the mouse or the text bottom
   (height * 1.5 + scrollY > contentHeight) sets the screen close request sv_PopRequest; the frame
   skip of the scroll is the frame rate sv_FramesPerTick 0x62378c (>= 80: 4 rows, >= 30: 2, below: 1
   per frame). A second fade loop (the new black cover, .., 0x3c, 1) scrolls on until that cover ends;
   then the window is removed (d2_RemoveWindow 0x472ce0), the picture 0x797 is shown, the cover
   released and the final black cover (.., 0x3c, 10) drawn with d2_CutsceneActive 0x6ed9d4 cleared. */

void ai_CreditsScreen(void)
{
    D2FADEVIEW *fade;
    int bar, frames, pump, step;

    step = 4;
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 1);
    while ((fade->flags & 4) == 0)
        sv_RunFrame(0, 0x24087);
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 10);
    d2_ClearScreen();
    d2_ShowPicture(0x7ad);
    bar = d2_CreateWindow(100, 0, 600, d2_ScreenH, 0x10);
    d2_PlaceWindowScaled(bar, 1);
    io_PrintText(0x2004);
    d2_LastWindowPtr->scrollY = -d2_ScreenH;
    frames = 0;
    d2_CutsceneActive = 1;
    pump = sv_RunFrame(ai_CreditsScreen, 0x24087);
    while (pump != 0) {
        if (dm_L.rup != 0 || kb_Last != 0)
            sv_PopRequest = 1;
        d2_LastWindowPtr = &d2_Wins[bar];
        if (frames % step == 0)
            d2_LastWindowPtr->scrollY = d2_LastWindowPtr->scrollY + 1;
        frames++;
        if ((double)d2_LastWindowPtr->h * 1.5 + (double)d2_LastWindowPtr->scrollY >
            (double)d2_LastWindowPtr->contentHeight)
            sv_PopRequest = 1;
        if (sv_FramesPerTick >= 80.0f)
            step = 4;
        else if (sv_FramesPerTick >= 30.0f)
            step = 2;
        else if (sv_FramesPerTick < 30.0f)
            step = 1;
        pump = sv_RunFrame(ai_CreditsScreen, 0x24087);
    }
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 1);
    while ((fade->flags & 4) == 0) {
        sv_RunFrame(0, 0x24087);
        d2_LastWindowPtr = &d2_Wins[bar];
        if (frames % step == 0)
            d2_LastWindowPtr->scrollY++;
        if (sv_FramesPerTick >= 80.0f)
            step = 4;
        else if (sv_FramesPerTick >= 30.0f)
            step = 2;
        else if (sv_FramesPerTick < 30.0f)
            step = 1;
        frames++;
    }
    d2_RemoveWindow(bar);
    d2_ShowPicture(0x797);
    d2_ReleaseFade(fade);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x3c, 10);
    d2_CutsceneActive = 0;
}
