/* 0x460e50, reloads a texture (after a mode change); keep: the software pixels are kept */

void tex_Reload(TSTEXTURE *tex, char keep)
{
    TSTEXTURE *t;
    unsigned int i;
    unsigned char skip;
    char *path;

    if (!tex || tex->refs <= 0)
        return;
    if (tex->alias)
        tex = &d3_Textures[tex->mask];
    skip = d3_MipSkip;
    if (d3_Hardware) {
        if (tex->surf && !keep)
            goto done;
        if (tex->surf) {
            d3_ddhr = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
        if (tex->d3dtex) {
            d3_ddhr = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (keep && tex->data) {
            mem_Free(tex->data);
            tex->data = 0;
        }
        if (tex->mask && tex->name[0] != '*' && (path = tex_FindFile(tex->name))) {
            i = tex->nomip ? 0 : ts_Mips;
            d3_MipSkip = tex->noscale ? 0 : skip;
            if (tex->hicolor) {
                d3d_LoadTexture(path, &d3_TexFormat16, &tex->surf, &tex->d3dtex, i, tex->origsize, tex->origsize, tex->filebpp);
                tex->bpp = d3_TexFormat16Bpp;
            } else {
                d3d_LoadTexture(path, &d3_TexFormat8, &tex->surf, &tex->d3dtex, i, tex->origsize, tex->origsize, tex->filebpp);
                tex->bpp = d3d_UsedTexFormat.bpp;
            }
            tex->size = tex->origsize >> d3_MipSkip;
        }
    } else {
        if (!tex->surf && !tex->d3dtex && tex->data && !keep)
            goto done;
        if (tex->surf) {
            d3_ddhr = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
        if (tex->d3dtex) {
            d3_ddhr = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (!keep && tex->data)
            mem_Free(tex->data);
        tex->data = 0;
        if (tex->name[0] == '*')
            tex_Release(tex);
        else if (tex->mask) {
            ts_LoadSoft(tex_FindFile(tex->name), tex);
            tex->bpp = 8;
        }
    }
done:
    ts_SyncAliases(tex);
    d3_MipSkip = skip;
    if (tex->frames > 1 && !tex->frame) {
        for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && t->group == tex->group && t->frame > 0)
                tex_Reload(t, keep);
        }
    }
}
