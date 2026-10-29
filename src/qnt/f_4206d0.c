/* 0x4206d0, quantizes the w x h RGB image to ncolors (octree, optional error diffusion): writes the
   palette indices to out and the 256-entry palette (3 x 256 bytes, planar) to pal; 0 on out of memory */
int qnt_Quantize(unsigned char *rgb, unsigned char *out, int w, int h, int ncolors, int dither, unsigned char *pal)
{
    if (!qnt_Inited) {
        qnt_Inited = 1;
        qnt_InitTables();
    }
    if (!qnt_Alloc()) {
        qnt_Free();
        return 0;
    }
    if (!qnt_Build(rgb, w * h)) {
        qnt_Free();
        return 0;
    }
    qnt_Reduce(ncolors);
    qnt_MakePalette(0, 0);
    if (!qnt_Map(rgb, out, w, h, dither)) {
        qnt_Free();
        return 0;
    }
    qnt_Free();
    qnt_GetPalette(pal);
    return 1;
}
