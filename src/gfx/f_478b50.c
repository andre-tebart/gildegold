/* 0x478b50, makes a 24 bit surface of a bmp, 0 on failure */

D2SURF *d2_LoadBmpSurface(char *name)
{
    D2SURF d;
    D2SURF *s;
    unsigned int v;
    int w, h;

    memset(&d, 0, sizeof(d));
    v = d2_GetBmpSize(name);
    if ((int)v < 0)
        return 0;
    w = v & 0xffff;
    h = (int)v >> 16;
    if (h < 0)
        h = abs(h);
    d.h = h;
    d.w = w;
    d.bpp = 0x18;
    s = d2_MakeSurface(&d, 0);
    if (s == 0)
        return 0;
    d2_ReadBmpPixels(name, (unsigned char *)s->bits);
    return s;
}
