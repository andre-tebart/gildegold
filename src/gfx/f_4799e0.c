/* 0x4799e0, plots the pixel (x, y) of surf in the colour (r, g, b) at the surface's depth (8 bpp: the grey
   average; 15 / 16 bpp: dd_RGB; 24 bpp: 3 bytes; 32 bpp: a dword), clipped to the surface's clip rectangle;
   1 if plotted, 0 when outside the clip or surf is NULL (the pen d2_Plot and the outline below use it) */

int d2_PutPixelRGB(int x, int y, unsigned int r, unsigned int g, unsigned int b, D2SURF *surf)
{
    unsigned char bpp;
    DWORD c;

    if (surf != 0 && x >= surf->left && x < surf->right && y >= surf->top && y < surf->bottom) {
        bpp = surf->bpp >> 3;
        c = dd_RGB((unsigned char)r, (unsigned char)g, (unsigned char)b);
        switch (surf->bpp) {
        case 8:
            ((unsigned char *)surf->bits)[surf->pitch * y + x] =
                (unsigned char)((int)((r & 0xff) + (g & 0xff) + (b & 0xff)) / 3);
            break;
        case 0xf:
        case 0x10:
            ((unsigned short *)surf->bits)[surf->pitch * y + x] = (unsigned short)c;
            break;
        case 0x18:
            x = bpp * x;
            ((unsigned char *)surf->bits)[surf->pitch * y + x] = (unsigned char)r;
            ((unsigned char *)surf->bits)[surf->pitch * y + x + 1] = (unsigned char)g;
            ((unsigned char *)surf->bits)[surf->pitch * y + x + 2] = (unsigned char)b;
            break;
        case 0x20:
            ((DWORD *)surf->bits)[surf->pitch * y + x] = c;
        }
        return 1;
    }
    return 0;
}
