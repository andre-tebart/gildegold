/* 0x43f910, reloads the textures of a map (does nothing for a scene without one) */

void d3fl_ReloadMap(void *p)
{
    if (p)
        d3fl_ReloadTextures(p);
}
