/* 0x448200, loads a sprite texture (alpha 0x7f) */

D3TEXHANDLE *d3if_LoadSprite(char *name, char glow)
{
    D3IFTEXFLAGS f;
    D3TEXHANDLE *t;

    memset(&f, 0, sizeof f);
    f.bits.alpha = 0x7f;
    f.bits.glow = glow;
    if (d3d_Device && (d3d_UsedCard.info.caps & 1)) {
        t = (D3TEXHANDLE *)tex_Load(name, f.raw, 0, 0);
        if (t)
            tex_Reload(t, 0);
        return t;
    }
    return 0;
}
