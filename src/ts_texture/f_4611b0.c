/* 0x4611b0, switches a texture between the 8-bit and the 16-bit surface format */

char tex_SetHiColor(TSTEXTURE *tex, unsigned char hicolor)
{
    if (tex && d3_Hardware) {
        if (tex->alias)
            tex = &d3_Textures[tex->mask];
        if (d3_TexFormat8.dwRBitMask != d3_TexFormat16.dwRBitMask || d3_TexFormat8.dwGBitMask != d3_TexFormat16.dwGBitMask
            || d3_TexFormat8.dwBBitMask != d3_TexFormat16.dwBBitMask) {
            if (tex->hicolor != hicolor) {
                tex->hicolor = hicolor;
                return tex_Unload(tex);
            }
        }
    }
    return 0;
}
