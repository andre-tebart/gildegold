/* 0x43f180, creates the texture cache of n empty entries, builds the three blend weight tables and selects
   the MMX blend routine (the C one without MMX) */

void d3fl_InitCache(unsigned int n)
{
    unsigned int i;

    d3fl_Count = n;
    d3fl_Cache = mem_Alloc(n * sizeof(D3FLCACHE), "d3_fl:TextureCache");
    for (i = 0; i < d3fl_Count; i++) {
        d3fl_Cache[i].tex = 0;
        d3fl_Cache[i].flag = 0;
        d3fl_Cache[i].stamp = 0;
        d3fl_Cache[i].size = 0;
        D3_ZERO(d3fl_Cache[i].t.tiles, sizeof(d3fl_Cache[i].t.tiles));
    }
    d3fl_MakeWeights((unsigned char *)d3fl_W64, 64);
    d3fl_MakeWeights((unsigned char *)d3fl_W32, 32);
    d3fl_MakeWeights((unsigned char *)d3fl_W16, 16);
    d3fl_Blend = d3fl_BlendMMX;
    if (!dd_HasMMX)
        d3fl_Blend = d3fl_BlendC;
}
