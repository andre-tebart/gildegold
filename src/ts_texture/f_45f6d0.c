/* 0x45f6d0 */

void ts_CacheStore(TSCACHE *sce, TSTEXTURE *tex)
{
    if (sce->size > 0) {
        if (sce->d3dtex) {
            d3_ddhr = sce->d3dtex->lpVtbl->Release(sce->d3dtex);
            sce->d3dtex = 0;
        }
        if (sce->surf) {
            d3_ddhr = sce->surf->lpVtbl->Release(sce->surf);
            sce->surf = 0;
        }
    }
    sce->hicolor = tex->hicolor;
    sce->time = d3_FrameCount;
    sce->size = tex->size;
    sce->surf = tex->surf;
    sce->nomip = tex->nomip;
    if (sce->surf) {
        if (tex->bpp == 8) {
#line 133
            D3DCHECK(sce->surf->lpVtbl->SetPalette(sce->surf, NULL));
            sce->d3dtex = tex->d3dtex;
        } else
            sce->d3dtex = tex->d3dtex;
    } else
        sce->d3dtex = tex->d3dtex;
}
