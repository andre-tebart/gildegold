/* 0x44a170, sets the fog between fog sets a and b (t = 0..1, 0 = a) */

char d3_SetFogSet(unsigned int a, unsigned int b, float t)
{
    D3FOGCOL col;

    if (a < 6 && b < 6 && t >= 0.0f && t <= 1.0f) {
        col.r = (int)((d3o_Fog[b].col.r - d3o_Fog[a].col.r) * t + d3o_Fog[a].col.r);
        col.g = (int)((d3o_Fog[b].col.g - d3o_Fog[a].col.g) * t + d3o_Fog[a].col.g);
        col.b = (int)((d3o_Fog[b].col.b - d3o_Fog[a].col.b) * t + d3o_Fog[a].col.b);
        d3_SetFog(((d3o_Fog[b].start - d3o_Fog[a].start) * t + d3o_Fog[a].start) * d3o_FogScale,
                  ((d3o_Fog[b].end - d3o_Fog[a].end) * t + d3o_Fog[a].end) * d3o_FogScale,
                  col);
        d3o_FogSet = a;
        return 1;
    }
    return 0;
}
