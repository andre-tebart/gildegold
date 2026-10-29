/* 0x530900, sets the screen centre 0xf8f08c / 0xf8f088: with mask bit 0x40 the centre of the 3D viewport
   (d3_ViewW * 0.5f + d3_ViewX, d3_ViewH * 0.5f + d3_ViewY), shifted by -6 / +10 screen-scale pixels while the
   left button is held (mask bit 0x100); else half the screen size (0xfb9072 / 0xfb9070). Read from
   0x530900..0x530992: the two floats at 0xfe73c8 / 0xfe73c4 times the 0.5f at 0x5eb7ac, plus the view origins
   0xfe73a8 / 0xfc5328, and the 6.0f (0x5ebb1c) / 10.0f (0x5ebb68) terms times d2_ScreenScale (0x5fe708) in
   the bit 0x100 arm. The ftol of the x float is hoisted above the y computation by the original's scheduler. */

void sv_SetScreenCenter(unsigned fl)
{
    if ((fl & 0x40) != 0) {
        int w, h;

        if ((fl & 0x100) != 0) {
            w = (int)(*(float *)(unsigned)&d3_ViewW * 0.5f + *(float *)(unsigned)&d3_ViewX
                      - *(float *)(unsigned)&ai_Unk5fe708 * 6.0f);
            h = (int)(*(float *)(unsigned)&d3_ViewH * 0.5f + *(float *)(unsigned)&ai_Unk5fe708 * 10.0f
                      + *(float *)(unsigned)&d3_ViewY);
        } else {
            w = (int)(*(float *)(unsigned)&d3_ViewW * 0.5f + *(float *)(unsigned)&d3_ViewX);
            h = (int)(*(float *)(unsigned)&d3_ViewH * 0.5f + *(float *)(unsigned)&d3_ViewY);
        }
        SV_I((unsigned)&d2_HalfScreenW) = w;
        SV_I((unsigned)&d2_HalfScreenH) = h;
        return;
    }
    SV_I((unsigned)&d2_HalfScreenW) = (int)SV_S((unsigned)&d2_ScreenW) >> 1;
    SV_I((unsigned)&d2_HalfScreenH) = (int)SV_S((unsigned)&d2_ScreenH) >> 1;
}
