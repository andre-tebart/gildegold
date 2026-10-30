/* 0x4613b0, an empty 16-bit texture (hardware only); the order of the field stores is needed for the bytes
   (VC6 sinks the bitfield stores f0 / hicolor / nomip and f6a behind the zero stores) */

TSTEXTURE *tex_CreateHi(char *name, int size, TSFLAGS flags, unsigned char f0, unsigned char f6a, char mips)
{
    TSTEXTURE *tex;
    int index;

    tex = ts_AllocSlot(flags, &index);
    if (tex && d3_HardwareC) {
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
        tex->hicolor = 1;
        tex->nomip = !mips;
        tex->refs = 1;
        tex->anim = 0;
        tex->group = -1;
        tex->bpp = d3_TexFormat16Bpp;
        ts_TexUsed++;
        return tex;
    }
    return 0;
}
