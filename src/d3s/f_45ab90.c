/* 0x45ab90, frees the shadow texture cache */

void d3s_CacheFree(void)
{
    d3s_CacheAge = 0;
    d3s_CacheCount = 0;
    mem_Free(d3s_Cache);
    d3s_Cache = 0;
}
