/* 0x43c4e0, applies the graphics options: the mip mode (a), the texture detail (b), the texture shift (c)
   and the shadow mode; a changed mode resets the object shadows, a changed shift flushes and reloads the
   textures (e = reload flag). */

void d3_ApplyGraphicsOptions(int a, unsigned char b, unsigned char c, unsigned char mode, int e)
{
    if (d3_ShadowMode.mode != mode || d3d_TexShift != c) {
        d3_ShadowMode.mode = mode;
        d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, d3s_ResetShadows);
    }
    d3fl_SetMips(a);
    d3fl_SetDetail(b);
    if (d3d_TexShift != c) {
        d3fl_Flush();
        d3d_TexShift = c;
        tex_ReloadAll(1, e);
    }
}
