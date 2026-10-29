/* 0x46e150, draws all objects and shows the frame */

void d2_DrawAll(void)
{
    d2_FlushDirty();
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_DrawObjects();
        d2_UnlockSurface(d2_ScreenSurf);
        dd_Flip();
    }
}
