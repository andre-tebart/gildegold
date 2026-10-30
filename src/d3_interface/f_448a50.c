/* 0x448a50, clears the viewport (and refreshes the screen) when the scene wants it: with the fog colour
   unless the back buffer was just cleared */

void d3if_ClearView(void)
{
    if (d3_EngineReady && d3_Busy <= 0) {
        if (d3_Hardware) {
            d3d_Clear(&d3_Viewport);
        } else {
            if (!dd_Cleared)
                dd_ClearB(&d3_Viewport, GetBValue(d3_FogColor), GetGValue(d3_FogColor), GetRValue(d3_FogColor));
        }
        d3_Frame(0x40, 1, 1);
    }
}
