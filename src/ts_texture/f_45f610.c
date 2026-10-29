/* 0x45f610 */
void ts_InitCache(unsigned int size)
{
    ts_CacheSize = size;
    ts_Unkfb91b4 = 0;
    ts_CacheUsed = 0;
    if (size > 0)
        ts_Cache = mem_Alloc(size * sizeof(TSCACHE), "d3_ts:SurfaceCache");
    else
        ts_Cache = 0;
}
