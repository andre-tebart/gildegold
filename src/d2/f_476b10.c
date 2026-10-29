/* 0x476b10, draws all fades */

void d2_DrawFades(void)
{
    int i;

    for (i = 0; i < 0x20; i++)
        if (d2_Fades[i] != 0) {
#ifdef REA_FRAME_DUMP
            REA_DUMP_TRACE((rd_f, "%u Fade %d\n", GetTickCount(), i));
#endif
            d2_DrawFade(d2_Fades[i], d2_ScreenSurf);
        }
}
