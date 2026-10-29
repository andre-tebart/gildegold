/* 0x46ea10, width in pixels of text in the font with index font ('~' is skipped) */

int d2_TextWidth(char *text, int font)
{
    D2PICHDR *f;
    D2FRAME *g;
    int i, w, len;
    unsigned char c;
    w = 0;
    len = strlen(text);
    f = d2_GetPic(font);
    for (i = 0; i < len; i++) {
        c = text[i];
        if (c != '~') {
            g = d2_GetFrame(f, c);
            if (i != 0 && text[i - 1] != ' ')
                w -= g->kern;
            if (c != ' ')
                w += g->adv + d2_LetterGap;
            else
                w += g->adv + d2_SpaceWidth + d2_LetterGap;
        }
    }
    return d2_LetterGap + w;
}
