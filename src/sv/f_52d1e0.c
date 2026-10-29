/* 0x52d1e0, draws the name plates of the objects of the table 0x760640 (0x32 ints each, name at +0x48) above their screen boxes, then walks the scene objects of kind 0x20 with the callback 0x52d010 */

void sv_Unk52d1e0(void)
{
    int *p, o, t[3], x;

    d2_SetFont(0x54);
    p = (int *)(unsigned)&sv_NamePlates;
    do {
        o = *p;
        if (o != 0 && (*(int *)(o + 0x1cc) != 0 || *(int *)(o + 0x218) == 0)) {
            if ((char)d3if_ScreenBox(o, t) != 0) {
                x = (t[2] - t[0]) / 2 - 0x50 + t[0];
                d2_DrawTextBox(x, t[1], 0xa0, 0x60, d2_ScreenSurf, (char *)(p + 0x12), 0x28);
                d2_AddDirty(x, t[1], SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
            }
        }
        p += 0x32;
    } while ((int)p < (unsigned)((unsigned)sv_NamePlates + 0x1900));
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0x20, sv_Unk52d010);
}
