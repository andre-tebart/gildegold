/* 0x43a610, sets the fog range: the fog scale (255 / (end - start)), the squared start, and when the
   fog is off, shrinks the far plane to the fog end; then refreshes all objects.
   @deep Q2: d3_Unkfc2df0 / d3_Unkfc2df4 (0xfc2df0, 0xfc2df4) are only written from d3_SetViewport's last two int parameters at 0x43b950 and passed back unchanged by both of its callers (d3_SetFogRange 0x43a610, d3_SetFog 0x43a6f0); nothing else reads them, the data has no initial value and no caller ever varies them, so there is no evidence for a role name */

void d3_SetFogRange(float start, float end)
{
    float zf;

    if (d3_FogStart != start || d3_FogEnd != end) {
        d3_FogStart = start;
        d3_FogScale = 255.0f / (end - start);
        d3_FogEnd = end;
        d3_FogStart2 = start * start;
        if (d3_FogOn)
            zf = d3_SavedZFar;
        else
            zf = d3_SavedZFar = d3_ZFar;
        if (end < zf)
            d3_SetViewport(d3_ViewW, d3_ViewH, d3_ViewX, d3_ViewY, d3_ZNear, end, d3_ScaleX, d3_Unkfc2df4, d3_Unkfc2df0);
        d3_UpdateAll(0);
    }
}
