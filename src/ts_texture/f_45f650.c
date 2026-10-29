/* 0x45f650 */

void ts_FreeCache(void)
{
    TSCACHE *sce;
    unsigned int i;

    if (ts_Cache) {
        for (i = 0, sce = ts_Cache; i < ts_CacheSize; i++, sce++) {
            if (sce->size > 0) {
                if (sce->d3dtex) {
                    d3_ddhr = sce->d3dtex->lpVtbl->Release(sce->d3dtex);
                    sce->d3dtex = 0;
                }
                if (sce->surf) {
                    d3_ddhr = sce->surf->lpVtbl->Release(sce->surf);
                    sce->surf = 0;
                }
                sce->size = 0;
            }
        }
        mem_Free(ts_Cache);
        ts_Cache = 0;
    }
    ts_CacheSize = 0;
    ts_CacheUsed = 0;
}
