/* 0x46f000, draws text at (x, y) in the font "_FONT"; flags 4: shadow, 2 and 1: extra passes; '~' is skipped;
   stops at the right screen edge; leaves the size drawn in d2_TextDrawnW / d2_TextDrawnH */

void d2_DrawText(int x, int y, void *surf, char *text, unsigned char flags)
{
    D2FRAME *g;
    unsigned int i;
    int x0;
    unsigned char c;

    x0 = x;
    if (d2_FontPic == 0)
        d2_FontPic = d2_GetPic(d2_FindPic("_FONT"));
    d2_FontHeight = d2_FontPic->h;
    for (i = 0; i < strlen(text); i++) {
        c = text[i];
        g = d2_GetFrame(d2_FontPic, c);
        x -= g->kern;
        if (c != '~') {
            if (c != ' ') {
                if (flags & 4)
                    d2_DrawShapeMode2(x + 6, y + 6, surf, d2_FontPic, c);
                d2_DrawShape(x, y, surf, d2_FontPic, c);
                if (flags & 2)
                    d2_DrawShapeMode3(x, y, surf, d2_FontPic, c);
                if (flags & 1)
                    d2_DrawShapeMode2(x, y, surf, d2_FontPic, c);
                x += g->adv + d2_LetterGap;
            } else
                x += d2_SpaceWidth;
        }
        if (x >= d2_ScreenW)
            break;
    }
    d2_TextDrawnH = d2_FontHeight;
    d2_TextDrawnW = x - x0;
}
