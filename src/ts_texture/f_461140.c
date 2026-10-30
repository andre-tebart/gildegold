/* 0x461140, drops the surfaces of a texture and reloads it */

char tex_Unload(TSTEXTURE *tex)
{
    if (tex && d3_HardwareC && !tex->alias) {
        if (tex->surf) {
            d3_ddhr = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
        if (tex->d3dtex) {
            d3_ddhr = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (tex->mask && tex->name[0] != '*')
            tex_Reload(tex, 0);
    }
    return 0;
}
