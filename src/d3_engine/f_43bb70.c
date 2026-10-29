/* 0x43bb70, switches the hardware renderer on / off: switching on flushes the texture cache and
   re-inits the sky; when the mode changed it updates all objects, reloads the textures and the lights. */

void d3_SetHardwareMode(int on)
{
    int old = d3_HardwareBits.on;

    if (d3_D3DActive && on) {
        d3_HardwareBits.on = 1;
        if (d3_EngineReady)
            d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3s_FreeShadows);
        d3fl_Flush();
        if (d3_Sky)
            d3sky_Init(d3_Sky, 1);
    } else
        d3_HardwareBits.on = 0;
    if (old != d3_HardwareBits.on && d3_EngineReady) {
        d3_UpdateAll(0);
        tex_ReloadAll(1, 0);
        d3_LightInvalidate(1);
    }
}
