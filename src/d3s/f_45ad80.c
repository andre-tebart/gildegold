/* 0x45ad80, sets up the shadow system (texture sizes, poly buffers, cache, lights) */

void d3_InitShadows(unsigned int size, unsigned int maxsize, int ncache, int age, char mips, int polys, int mapsize)
{
    if (maxsize >= d3d_UsedCard.info.maxw)
        maxsize = d3d_UsedCard.info.maxw;
    d3s_MaxTexSize = maxsize;
    d3s_TexSize = size;
    if (size >= maxsize)
        d3s_TexSize = maxsize;
    d3s_MaxPolys2 = polys;
    d3s_MaxPolys = polys * polys;
    d3s_ShadowMapSize = mapsize;
    d3s_UVs = mem_Alloc(d3s_MaxPolys * 0x30, "d3s:BuildingPolyBuffer(Shadow)");
    d3s_NumUVs = 0;
    d3s_Verts = mem_Alloc((d3s_MaxPolys >> 4) * 600, "d3s:BuildingPointBuffer(Shadow)");
    d3s_Enabled = 1;
    d3s_LodTexSize = 1;
    d3s_NumVerts = 0;
    d3s_QuadUV[0] = 0.0f;
    d3s_QuadUV[1] = 1.0f;
    d3s_QuadUV[2] = 1.0f;
    d3s_QuadUV[3] = 1.0f;
    d3s_QuadUV[4] = 0.0f;
    d3s_QuadUV[5] = 0.0f;
    d3s_QuadUV2[0] = 1.0f;
    d3s_QuadUV2[1] = 1.0f;
    d3s_QuadUV2[2] = 1.0f;
    d3s_QuadUV2[3] = 0.0f;
    d3s_QuadUV2[4] = 0.0f;
    d3s_QuadUV2[5] = 0.0f;
    d3s_ShadowMips = mips;
    d3s_CacheInit(ncache, age);
    d3s_NumLights = 0;
    memset(d3s_Lights, 0, 16);
}
