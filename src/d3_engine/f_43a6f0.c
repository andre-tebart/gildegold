/* 0x43a6f0 @deep Q2 blocked like d3_SetFogRange: d3_Unkfc2df0 / d3_Unkfc2df4 are only stored from d3_SetViewport's last two int parameters and passed back, no role evidence; behaviour draft 50 code hunks: the original duplicates the four tail stores (FogEnd2, FogOn, FogStart1, FogColor) in both branches at 0x43a72b..0x43a74b and 0x43a7a1..0x43a7b3 while VC6 shares one tail after a jmp in ours; moving the stores into both branches was tried and made it worse (74 -> 89 differing bytes) */

void d3_SetFog(float start, float end, unsigned long color)
{
    char on;

    if (d3_Hardware && (d3d_Caps98d & 0x20)) {
        if (start < end) {
            on = 1;
            d3_SetFogRange(start, end);
        } else {
            on = 0;
            if (d3_FogOn)
                d3_SetViewport(d3_ViewW, d3_ViewH, d3_ViewX, d3_ViewY, d3_ZNear, d3_SavedZFar, d3_ScaleX, d3_Unkfc2df4, d3_Unkfc2df0);
        }
        d3_FogOn = on;
        d3_FogStart1 = start;
        d3_FogEnd2 = end;
        d3_FogColor = color;
    }
}
