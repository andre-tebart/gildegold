/* 0x41df80 */

char dd_LockNoWait(void)
{
    switch (dd_Mode) {
    case DD_WINDOWMEM:
    case DD_FULLMEM:
        dd_Surface = dd_MemBuff;
        dd_Pitch = dd_MemPitch;
        dd_PitchPixels = dd_Width;
        return 1;
    case DD_GDI:
        if (!dd_MemBottom) {
            dd_Surface = dd_MemBuff;
            dd_Pitch = dd_MemPitch;
            dd_PitchPixels = dd_Width;
        } else {
            dd_Surface = dd_MemBottom;
            dd_Pitch = -dd_MemPitch;
            dd_PitchPixels = -dd_Width;
        }
        return 1;
    case DD_WINDOW:
    case DD_FULLSCREEN:
        dd_Surface = dd_LockSurfaceFlags(dd_BackBuffer, 0);
        return dd_Surface != NULL;
    }
    return 0;
}
