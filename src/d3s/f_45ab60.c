/* 0x45ab60, allocates n shadow texture cache entries */

void d3s_CacheInit(int n, int age)
{
    d3s_CacheCount = n;
    d3s_CacheAge = age;
    d3s_Cache = mem_Alloc(n * 0x14, "d3s:Cache");
}
