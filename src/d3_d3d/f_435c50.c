/* 0x435c50, fills the global d3d_UsedTexFormat for later texture loads: enumerates the device's
   texture formats into pf (closest to bpp, or the wanted fourcc when the device has it) and pf2 (the
   second choice for bpp2), then stores the resulting bit counts (4 for DXT1) and whether pf and pf2
   agree; returns nothing */

void d3d_FindTextureFormats(DWORD bpp, DWORD fourcc, DWORD bpp2)
{
    D3_ZERO_AT(offset d3d_UsedTexFormat, sizeof(d3d_UsedTexFormat));
    d3d_UsedTexFormat.bpp = bpp;
    d3d_UsedTexFormat.fourcc = fourcc;
    d3d_UsedTexFormat.bpp2 = bpp2;
    d3d_UsedTexFormat.found = 0;
    d3d_UsedTexFormat.exact = 0;
#line 195
    D3DCHECK(d3d_Device->lpVtbl->EnumTextureFormats(d3d_Device, d3d_EnumTextureFormatsCallback, &d3d_UsedTexFormat));
    if (d3d_UsedTexFormat.fourcc && !d3d_UsedTexFormat.found)
        d3d_UsedTexFormat.fourcc = 0;
    d3d_UsedTexFormat.bpp = d3d_UsedTexFormat.pf.dwRGBBitCount;
    if (d3d_UsedTexFormat.fourcc == MAKEFOURCC('D', 'X', 'T', '1'))
        d3d_UsedTexFormat.bpp = 4;
    d3d_UsedTexFormat.bpp2 = d3d_UsedTexFormat.pf2.dwRGBBitCount;
    if (d3d_UsedTexFormat.pf.dwRGBBitCount == d3d_UsedTexFormat.pf2.dwRGBBitCount
        && d3d_UsedTexFormat.pf.dwRBitMask == d3d_UsedTexFormat.pf2.dwRBitMask
        && d3d_UsedTexFormat.pf.dwGBitMask == d3d_UsedTexFormat.pf2.dwGBitMask
        && d3d_UsedTexFormat.pf.dwBBitMask == d3d_UsedTexFormat.pf2.dwBBitMask)
        d3d_UsedTexFormat.same = 1;
    else
        d3d_UsedTexFormat.same = 0;
}
