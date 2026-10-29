/* 0x45f980 */

TSTEXTURE *tex_Find(char *name)
{
    char buf[0x40];
    TSTEXTURE *tex;
    unsigned int i;

    strcpy(buf, name);
    for (i = 0, tex = d3_Textures; i < d3_TexCount; i++, tex++) {
        if (tex->refs > 0 && !_strcmpi(buf, tex->name))
            return tex;
    }
    return 0;
}
