/* 0x46e990, clears the screen (fades drawn), flips, clears both surfaces again */

void d2_ClearScreen(void)
{
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_DrawFades();
    dd_Flip();
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_ClearSurface(d2_BackSurf);
}
