/* 0x52e0c0, draws the status line 0x761f40 (screen centre, y by a float of the clock) until it is 0x15e ms old */

void sv_Unk52e0c0(void)
{
    int x, y;

    if (SV_B((unsigned)&sv_StatusText) != 0) {
        d2_SetFont(0x53);
        if ((SV_U((unsigned)&sv_FrameMask) & 0x8000) != 0) {
            x = SV_I((unsigned)&d2_HalfScreenW) - 300;
            y = (int)(*(float *)(unsigned)&ai_Unk5fe708 * 505.0f);
        } else {
            x = SV_I((unsigned)&d2_HalfScreenW) - 300;
            y = (int)(*(float *)(unsigned)&ai_Unk5fe708 * 575.0f);
        }
        d2_DrawTextBox(x, y, 600, 0x18, d2_ScreenSurf, (char *)(unsigned)&sv_StatusText, 0x28);
        d2_AddDirty(x, y, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        if (SV_U((unsigned)&sv_StatusTime) + 0x15e <= SV_U((unsigned)&ai_Unk6333cc))
            SV_B((unsigned)&sv_StatusText) = 0;
    }
}
