/* 0x479b60, plots a 16 bit pixel */

int d2_PutPixel(int x, int y, unsigned short color, D2SURF *surf)
{
    ((unsigned short *)surf->bits)[surf->pitch * y + x] = color;
    return 1;
}
