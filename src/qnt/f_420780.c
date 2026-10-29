/* 0x420780, copies the palette the last quantization built, planar R / G / B, 256 bytes each */

void qnt_GetPalette(unsigned char *pal)
{
    int i;

    for (i = 0; i < 256; i++) {
        pal[i] = qnt_PalR[i];
        pal[i + 256] = qnt_PalG[i];
        pal[i + 512] = qnt_PalB[i];
    }
}
