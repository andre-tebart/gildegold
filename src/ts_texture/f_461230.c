/* 0x461230, creates an empty texture slot (name, size, flags; hardware: fetched from the surface cache, software: data buffer
   and 16-bit colour table); the field stores are in this order for VC6's bitfield / zero-register code (see tex_CreateHi) */

TSTEXTURE *tex_Create(char *name, int size, char hicol, TSFLAGS flags, unsigned char f0, unsigned char f6a,
                      int npal, unsigned char *pal, char mips)
{
    unsigned char map[0x100];
    TSTEXTURE *tex;
    int index;
    int bpp;

    tex = ts_AllocSlot(flags, &index);
    if (!tex)
        return 0;
    D3_ZERO_AT(tex, sizeof(tex));
    strncpy(tex->name, name, 0x3f);
    tex->size = size;
    tex->origsize = size;
    tex->data = 0;
    tex->mask = size * size - 1 | size - 1;
    tex->flags = flags;
    tex->surf = 0;
    tex->d3dtex = 0;
    tex->src = 0;
    tex->unk58 = 0;
    tex->unk54 = 0;
    tex->f0 = f0;
    tex->f6a = f6a;
    tex->alias = 0;
    tex->hicolor = !hicol || d3d_UsedTexFormat.fourcc ? 1 : 0;
    tex->nomip = !mips;
    bpp = (unsigned char)(tex->hicolor ? d3_TexFormat16Bpp : d3d_UsedTexFormat.bpp);
    tex->refs = 1;
    tex->bpp = bpp;
    tex->filebpp = bpp;
    tex->anim = 0;
    tex->group = -1;
    ts_TexUsed++;
    if (d3_Hardware) {
        ts_CacheFetch(tex, npal, (int)pal);
        return tex;
    }
    tex->hicol = ts_GetHiColTab(pal, npal, map);
    tex->data = mem_Alloc(tex->size * tex->size, "d3_ts:LoadTextureSoft(data)");
    return tex;
}
