/* 0x46cfd0, redraws a picture and flips */

void d2_ShowPicture(int pic)
{
    d2_DrawPic(0, 0, pic, d2_ScreenSurf);
    d2_BlitPart(0, 0, ((D2SURF *)d2_ScreenSurf)->w, ((D2SURF *)d2_ScreenSurf)->h, d2_ScreenSurf, 0, 0, d2_BackSurf);
    d2_DrawFades();
    dd_Flip();
    d2_BlitPart(0, 0, ((D2SURF *)d2_ScreenSurf)->w, ((D2SURF *)d2_ScreenSurf)->h, d2_BackSurf, 0, 0, d2_ScreenSurf);
}
