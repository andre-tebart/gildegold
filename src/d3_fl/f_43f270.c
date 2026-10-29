/* 0x43f270, releases the cached textures, frees the cache and selects the C blend routine */

void d3fl_FreeCache(void)
{
    unsigned int i;

    for (i = 0; i < d3fl_Count; i++) {
        if (d3fl_Cache[i].tex) {
            if (d3fl_Cache[i].tex->name[0] == '*')
                while (d3fl_Cache[i].tex->refs > 0)
                    tex_Release((D3TEXHANDLE *)d3fl_Cache[i].tex);
            d3fl_Cache[i].tex = 0;
            d3fl_Cache[i].flag = 0;
        }
    }
    d3fl_Count = 0;
    mem_Free(d3fl_Cache);
    d3fl_Cache = 0;
    d3fl_Blend = d3fl_BlendC;
}
