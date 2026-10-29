/* 0x45fb50 */

void tex_Free(TSTEXTURE *tex)
{
    if (d3_Hardware && tex->name[0] == '*' && tex->surf && tex->d3dtex) {
        ts_CacheRelease(tex);
        tex->d3dtex = 0;
        tex->surf = 0;
    } else {
        if (tex->d3dtex) {
            d3_ddhr = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (tex->surf) {
            d3_ddhr = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
    }
    if (tex->data) {
        mem_Free(tex->data);
        tex->data = 0;
    }
}
