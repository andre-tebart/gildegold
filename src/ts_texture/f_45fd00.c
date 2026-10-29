/* 0x45fd00 */

void tex_Exit(void)
{
    TSTEXTURE *tex;
    unsigned int i;

    for (i = 0, tex = d3_Textures; i < d3_TexCount; i++, tex++) {
        while (tex->refs > 0)
            tex_Release(tex);
    }
    mem_Free(d3_Textures);
    d3_Textures = 0;
    for (i = 0; i < ts_HiColCount; i++) {
        mem_Free(ts_HiCol[i].tab);
        ts_HiCol[i].tab = 0;
    }
    ts_Palette = 0;
    if (ts_MaxHiCol > 0) {
        mem_Free(ts_HiCol);
        ts_HiCol = 0;
    }
    ts_HiColCount = 0;
    ts_TexUsed = 0;
    ts_Mips = 0;
    ts_FreeCache();
}
