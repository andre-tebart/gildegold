/* 0x41c420, DDBD_ bit depth flag of a pixel size */

DWORD dd_BppToDDBD(int bpp)
{
    if (bpp == 8)
        return DDBD_8;
    if (bpp == 16)
        return DDBD_16;
    if (bpp == 24)
        return DDBD_24;
    return bpp == 32 ? DDBD_32 : 0;
}
