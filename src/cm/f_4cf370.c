/* 0x4cf370, draws the scroll arrows (sprite ids 0 .. 7 of the sheet d2_GetPic(0x52c)): the mouse hit tests against
   the boxes at ai_Unkfc2dec + 0xb4 / 0xcc / 0xe4 / 0xfc pick the pressed (2) or the free (1) look. Rough draft. */

void ai_Unk4cf370()
{
    int e = 0, up = 0, a = 0, b = 0;
    int t;

    t = d2_GetPic(0x52c);
    if (ai_Unk6f22cc == -1) {
        up = 1;
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xfc), ai_Unk5ebba4, 0.2f) == 0)
            up = 2;
    } else if (ai_Unk6f22cc == 1) {
        e = 1;
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xe4), ai_Unk5ebba4, 0.2f) == 0)
            e = 2;
    } else if (ai_Unk6f22cc == 0) {
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xe4), ai_Unk5ebba4, 0.2f) == 0)
            e = 1;
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xfc), ai_Unk5ebba4, 0.2f) == 0)
            up = 1;
    }
    if (ai_Unk6f22d0 == -1) {
        a = 1;
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xb4), ai_Unk5ebba4, 0.2f) == 0)
            a = 2;
    } else if (ai_Unk6f22d0 == 1) {
        b = 1;
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xcc), ai_Unk5ebba4, 0.2f) == 0)
            b = 2;
    } else if (ai_Unk6f22d0 == 0) {
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xb4), ai_Unk5ebba4, 0.2f) == 0)
            a = 1;
        if (d3_VecEqual((float *)(ai_Unkfc2dec + 0xcc), ai_Unk5ebba4, 0.2f) == 0)
            b = 1;
    }
    d2_SetClipRect(0, 0, *(int *)((char *)d2_ScreenSurf + 4), *(int *)((char *)d2_ScreenSurf + 8));
    if (a == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(ai_Unk5fe708 * ai_Unk5edc50 - ai_Unk5eb81c), d2_ScreenSurf, t, 0);
    else if (a == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(ai_Unk5fe708 * ai_Unk5edc50 - ai_Unk5eb81c), d2_ScreenSurf, t, a);
    if (e == 2)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, e);
    else if (e == 1)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 3);
    if (up == 2)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 4);
    else if (up == 1)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 5);
    if (b == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(ai_Unk5fe708 * ai_Unk5eb7fc), d2_ScreenSurf, t, 6);
    else if (b == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(ai_Unk5fe708 * ai_Unk5eb7fc), d2_ScreenSurf, t, 7);
}
