/* 0x41e6d0, splits the packed color c into r / g / b through the pixel format's shifts and losses
   (dd_RShift / dd_GShift / dd_BShift, dd_RLoss / dd_GLoss / dd_BLoss) */

void dd_SplitRGB(DWORD c, unsigned char *r, unsigned char *g, unsigned char *b)
{
    *r = (unsigned char)(c >> dd_RShift) << dd_RLoss;
    *g = (unsigned char)(c >> dd_GShift) << dd_GLoss;
    *b = (unsigned char)(c >> dd_BShift) << dd_BLoss;
}
