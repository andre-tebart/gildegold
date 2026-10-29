/* 0x4616a0, the texture to use for a request with `flags`: a slot loaded without a fixed place (flags.b[0] 0xff,
   not from the back) gets its own alias when the request names a place, else tex itself */

TSTEXTURE *tex_AliasForFlags(TSTEXTURE *tex, TSFLAGS flags, char group)
{
    if (tex->flags.b[0] >= 0xff && !(tex->flags.b[2] & 1) && flags.b[0] != 0xff)
        return tex_CreateAlias(tex, flags, group);
    return tex;
}
