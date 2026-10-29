/* 0x45aee0, shuts the shadow system down */

void d3_ExitShadows(void)
{
    d3s_MaxTexSize = 0;
    d3s_TexSize = 0;
    d3s_MaxPolys2 = 0;
    d3s_MaxPolys = 0;
    d3s_NumUVs = 0;
    mem_Free(d3s_UVs);
    d3s_UVs = 0;
    d3s_NumVerts = 0;
    mem_Free(d3s_Verts);
    d3s_Verts = 0;
    d3s_CacheFree();
    if (d3sm_Shadow) {
        d3sm_Destroy(d3sm_Shadow);
        d3sm_Shadow = 0;
    }
}
