/* 0x52cef0, ages the speech bubble table 0x761fc0 (entries older than 300 ms are freed) and draws the rest above their objects (0x4486e0 gives the screen box; text: the entry's own or the default line
   0x143a1c0[+ -8]) */

void sv_Unk52cef0(void)
{
    int *p, o, ob, t[3], x, y;
    char *s;

    p = (int *)(unsigned)&sv_BubbleObjects;
    do {
        if (*p != 0 && (unsigned)(p[-1] + 300) < SV_U((unsigned)&ai_Unk6333cc))
            *p = 0;
        p += 0x43;
    } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
    d2_SetFont(0x54);
    p = (int *)(unsigned)&sv_BubbleObjects;
    do {
        o = *p;
        if (o != 0 && *(char **)((ob = *(int *)(o + 0x34)) + 0x208) == ai_Unk5fb5b0
            && (*(int *)(ob + 0x218) != 1 || ((ob = *(int *)(o + 0x22c)) != 0 && *(int *)(ob + 0x218) != 1))
            && (char)d3if_ScreenBox(ob, t) != 0) {
            y = t[1] - 0x40;
            x = (t[2] - t[0]) / 2 - 0x50 + t[0];
            s = (char *)(p + 1);
            if (*s == 0)
                s = (char *)SV_I((unsigned)&d2_Strings + p[-2] * 4);
            d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, s, 0x28);
            d2_AddDirty(x, y, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        }
        p += 0x43;
    } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
}
