/* 0x479df0, sets the colour format of the current display mode: the high-bit mask of a 16 bit pixel from the
   dd_ color shifts / losses and the same mask in both halves for the 32 bit (two pixel) reads, the palette
   depth and the light-table flag. With external 0 it (re)builds the light table with 0x30; the formatted debug
   text is unused. Returns 1. */

int d2_SetColourFormat(int external, int paletteDepth)
{
    char buf[132];

    if (external == 0)
        d2_MakeLightTable(0x30);
    d2_PaletteDepth = paletteDepth;
    d2_LightTableExternal = external;
    d2_ColourFormatSet = 1;
    d2_HighBitMask = (unsigned short)((((1 << (7 - dd_RLoss)) - 1) << dd_RShift)
                                    | (((1 << (7 - dd_GLoss)) - 1) << dd_GShift)
                                    | (((1 << (7 - dd_BLoss)) - 1) << dd_BShift));
    d2_HighBitMask32 = d2_HighBitMask | (d2_HighBitMask << 16);
    sprintf(buf, "HighBitMask:%i rShift:%i gShift:%i bShift:%i", d2_HighBitMask, dd_RShift, dd_GShift, dd_BShift);
    return 1;
}
