/* 0x43e1b0, end of a frame */

void d3_EndFrame(char tick)
{
    if (tick)
        d3_FrameCount++;
    d3_LightsChanged = 0;
    D3_MESH(d3s_Object)->surf[0].info.ntris = 0;
    D3_MESH(d3s_Object)->surf[0].info.nkeys = 0;
    d3s_NumUVs = 0;
    d3s_NumVerts = 0;
}
