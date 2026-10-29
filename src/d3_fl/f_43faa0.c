/* 0x43faa0, a free cache entry, or the least recently used one @deep Q2 only (byte already): D3FLTEX.unk58 (slot 0x58) has no role evidence - every use in the sources is a zero store (d3fl_Alloc eviction, d3fl_GetTexture creation, both d3fl_Draw re-render sites, ts_texture.c's tex_Load/tex_Create/tex_CreateHi initialisation, d3_tools' DC_ unload; TSTEXTURE.unk58 0x58 in ts_texture.h), no reader exists in the drafted source and the neighbouring 0x54 is the last-use stamp; callers (d3fl_GetTexture) and data (D3FLTEX, TSTEXTURE, D3TEXTURE views) read - a name would be invented */

D3FLCACHE *d3fl_Alloc(void)
{
    D3FLCACHE *c = d3fl_Cache, *best = 0;
    unsigned int i, oldest = d3_FrameCount;

    for (i = 0; i < d3fl_Count; i++, c++) {
        if (!c->tex) {
            c->stamp = d3_FrameCount;
            return c;
        }
        if (c->stamp < oldest) {
            oldest = c->stamp;
            best = c;
        }
    }
    if (best) {
        best->stamp = d3_FrameCount;
        if (best->tex)
            best->tex->unk58 = 0;
    }
    return best;
}
