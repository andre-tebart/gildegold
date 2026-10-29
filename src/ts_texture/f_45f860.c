/* 0x45f860 */

void ts_CreateSurface(TSTEXTURE *tex, int a, int b)
{
    unsigned char mips = tex->nomip ? 0 : ts_Mips;

    if (tex->hicolor)
        d3d_CreateTexture(tex->size, &d3_TexFormat16, &tex->surf, &tex->d3dtex, 0, 0, mips);
    else
        d3d_CreateTexture(tex->size, &d3_TexFormat8, &tex->surf, &tex->d3dtex, a, b, mips);
}
