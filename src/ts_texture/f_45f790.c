/* 0x45f790 */

void ts_CacheRelease(TSTEXTURE *tex)
{
    TSCACHE *sce, *best = 0;
    unsigned int i, oldest = d3_FrameCount;

    if (ts_CacheUsed == ts_CacheSize) {
        for (i = ts_CacheSize, sce = ts_Cache; i > 0; i--, sce++) {
            if (sce->size > 0 && sce->time < oldest) {
                oldest = sce->time;
                best = sce;
            }
        }
        if (best) {
            ts_CacheStore(best, tex);
            return;
        }
        if (tex->d3dtex) {
            d3_ddhr = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (tex->surf) {
            d3_ddhr = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
    } else {
        for (i = 0, sce = ts_Cache; i < ts_CacheSize; i++, sce++) {
            if (!sce->size) {
                ts_CacheStore(sce, tex);
                ts_CacheUsed++;
                return;
            }
        }
    }
}
