/* 0x476030, selects the font; 0 if the picture is no font */

void *d2_SetFont(int font)
{
    if (d2_Pics[font].type != 7)
        return 0;
    d2_FontPic = (D2PICHDR *)d2_GetPic(font);
    d2_CurrentFont = font;
    d2_FontHeight = d2_FontPic->h;
    if (font == d2_DefaultFont || font == d2_DefaultFont + 2)
        d2_SpaceWidth = 8;
    else
        d2_SpaceWidth = 4;
    d2_LetterGap = 2;
    return d2_FontPic;
}
