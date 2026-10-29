/* 0x45f8d0 */

void ts_CacheFetch(TSTEXTURE *tex, int a, int b)
{
    TSCACHE *sce;
    unsigned int i;

    if (ts_CacheUsed > 0) {
        for (i = 0, sce = ts_Cache; i < ts_CacheSize; i++, sce++) {
            if (sce->size == tex->size && (sce->hicolor == tex->hicolor || d3_Unkfe7a0d)
                && tex->nomip == sce->nomip) {
                sce->size = 0;
                ts_CacheUsed--;
                tex->surf = sce->surf;
                tex->d3dtex = sce->d3dtex;
                d3d_SetSurfacePalette(0, tex->surf, a, b);
                return;
            }
        }
    }
    ts_CreateSurface(tex, a, b);
}
