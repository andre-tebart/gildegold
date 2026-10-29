/* 0x45fe10, index of colour (r, g, b) in the table's palette; added (with its 63 shades) if missing */

unsigned char ts_AddColor(unsigned char r, unsigned char g, unsigned char b, TSHICOL *h)
{
    unsigned int i, j;

    for (i = 0; i < 0x100 - h->free; i++) {
        if (h->rgb[i][0] == r && h->rgb[i][1] == g && h->rgb[i][2] == b)
            break;
    }
    if (i == 0x100 - h->free) {
        h->tab[0x3f00 + i] = dd_RGB(r, g, b);
        h->rgb[i][0] = r;
        h->rgb[i][1] = g;
        h->rgb[i][2] = b;
        for (j = 0; j < 0x3f; j++)
            h->tab[(j << 8) + i] = dd_RGB(ts_Lerp(0, r, 62, j), ts_Lerp(0, g, 62, j), ts_Lerp(0, b, 62, j));
        h->free--;
    }
    return i;
}
