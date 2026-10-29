/* 0x4605c0, loads a texture or adds a reference to it; a loaded texture with other parameters is
   shared through an alias slot */

TSTEXTURE *tex_Load(char *name, TSFLAGS flags, unsigned char opts, unsigned char fmt)
{
    char upper[0x40];
    char copy[0x40];
    int h, w, bpp;
    TSTEXTURE *tex, *t, *a;
    unsigned int i;
    int ok;
    char *path;
    unsigned char f69 = fmt >> 4;
    unsigned char f6a = fmt & 0xf;
    unsigned char f5 = (opts & 0x40) && !(flags.b[0] >= 0xff && !(flags.b[2] & 1)) ? 1 : 0;
    unsigned char speed = (opts >> 2) & 0xf;
    unsigned char f4 = (opts >> 1) & 1;
    unsigned char f0 = opts & 1;

    tex = tex_Find(name);
    if (tex) {
        for (i = tex - d3_Textures, t = tex; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && !strcmp(name, t->name) && t->flags.all == flags.all && t->f0 == f0
                && t->f5 == f5 && t->f6a == f6a && t->f69 == f69 && t->f4 == f4 && t->speed == speed) {
                tex = t;
                break;
            }
        }
        if (tex == t) {
            tex_AddRef(tex, 1);
            return tex;
        }
        a = ts_AllocSlot(flags, 0);
        D3_ZERO_AT(a, sizeof(TSTEXTURE));
        strncpy(a->name, name, 0x3f);
        a->size = tex->size;
        a->origsize = tex->origsize;
        a->filebpp = tex->filebpp;
        a->data = tex->data;
        a->surf = tex->surf;
        a->d3dtex = tex->d3dtex;
        if (tex->alias)
            a->mask = tex->mask;
        else
            a->mask = tex - d3_Textures;
        a->unk58 = 0;
        a->unk54 = 0;
        a->flags = flags;
        a->f0 = f0;
        a->f5 = f5;
        a->f6a = f6a;
        a->f69 = f69;
        a->f4 = f4;
        a->alias = 1;
        a->hicolor = tex->hicolor;
        a->nomip = tex->nomip;
        a->noscale = tex->noscale;
        a->bpp = tex->bpp;
        a->anim = 0;
        a->refs = 1;
        a->speed = speed;
        a->group = -1;
        ts_TexUsed++;
        ts_LoadAnim(a, flags, opts, fmt);
        return a;
    }
    path = tex_FindFile(name);
    if (path)
        ok = bmp_GetInfo(path, &w, &h, &bpp);
    else {
        ok = 1;
        bpp = 0;
        h = 0;
        w = 0;
    }
    tex = ts_AllocSlot(flags, 0);
    strcpy(upper, name);
    strcpy(copy, name);
    _strupr(upper);
    if (ok && tex && w == h) {
        D3_ZERO_AT(tex, sizeof(TSTEXTURE));
        strncpy(tex->name, name, 0x3f);
        tex->data = 0;
        tex->size = w;
        tex->origsize = w;
        tex->mask = tex->size * tex->size - 1 | tex->size - 1;
        tex->filebpp = bpp;
        tex->unk58 = 0;
        tex->unk54 = 0;
        tex->flags = flags;
        tex->f5 = f5;
        tex->f0 = f0;
        tex->f69 = f69;
        tex->f6a = f6a;
        tex->f4 = f4;
        tex->alias = 0;
        tex->hicolor = !d3d_UsedTexFormat.fourcc && bpp > 8 ? 1 : 0;
        tex->nomip = strstr(upper, "_NM") != 0;
        if (tex->flags.b[2] & 2)
            tex->noscale = 1;
        else
            tex->noscale = strstr(upper, "_NS") != 0;
        tex->anim = 0;
        tex->refs = 1;
        tex->group = -1;
        tex->speed = speed;
        ts_TexUsed++;
        ts_LoadAnim(tex, flags, opts, fmt);
        return tex;
    }
    return 0;
}
