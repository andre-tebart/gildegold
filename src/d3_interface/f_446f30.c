/* 0x446f30, switches the texture set of a mesh part: loads the textures named in the part's table for
   `set` in place of the current ones (reloading them when `reload`) */

char d3if_SelectTextureSet(D3OBJECT *obj, D3SURF *surf, unsigned char set, char reload)
{
    char ok = 1;
    int i;
    D3FLTEX *old;
    D3TEXHANDLE *tex;
    char *name;
    int opts, fmt;
    char buf1[256], buf2[256];
    int j, replaced;
    D3OPS *ops;

    if (!obj || !D3_MESH(obj) || !surf)
        return 0;
    if (set == surf->texSet)
        return 1;
    obj->state |= 4;
    D3_MESH(obj)->mirrorSource = 0;
    if (D3_MESH(obj)->mirrorPoints) {
        mem_Free(D3_MESH(obj)->mirrorPoints);
        D3_MESH(obj)->mirrorPoints = 0;
    }
    if (D3_MESH(obj)->mirrorPolys) {
        mem_Free(D3_MESH(obj)->mirrorPolys);
        D3_MESH(obj)->mirrorPolys = 0;
    }
    ops = D3_OPS(&surf->info);
    if (set < ops->nsets) {
        for (i = 0; i < ops->count; i++) {
            surf->texSet = set;
            name = ops->texnames + (ops->count * set + i) * 0x40;
            if (*name && (old = ((D3FLTEX **)surf->texHandles)[i]) != 0) {
                tex_GetParams(old, &opts, &fmt);
                tex = (D3TEXHANDLE *)tex_Load(name, old->flags, opts, fmt);
                if (tex) {
                    D3PTRI *t;

                    replaced = 0;
                    if (reload)
                        tex_Reload(tex, 0);
                    ((D3TEXHANDLE **)surf->texHandles)[i] = tex;
                    t = (D3PTRI *)surf->info.tris;
                    for (j = 0; j < surf->info.ntris; j++, t++)
                        if (((D3SRCPOLY *)t->src)->slot == i) {
                            t->tex = (D3TEXTURE *)tex;
                            replaced++;
                        }
                    if (!replaced)
                        sprintf(buf2, "SelectTextureSet: no polys w/ %s found to replace w/ %s in %s", old->name, name, obj->name);
                    tex_Release((D3TEXHANDLE *)old);
                } else {
                    sprintf(buf1, "SelectTextureSet: cannot replace %s with %s (new not found) in %s", old->name, name, obj->name);
                    ok = 0;
                }
            }
        }
    } else
        ok = 0;
    return ok;
}
