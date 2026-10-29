/* 0x479480, d2_BlitPartMode without the colour key */

void d2_BlitPart(int sx, int sy, int w, int h, D2SURF *src, int x, int y, D2SURF *dst)
{
    d2_BlitPartMode(sx, sy, w, h, src, x, y, dst, 0);
}
