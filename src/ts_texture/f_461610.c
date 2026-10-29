/* 0x461610, releases an alias made by tex_CreateAlias; returns its source */

TSTEXTURE *tex_ReleaseAlias(TSTEXTURE *tex, char group)
{
    TSTEXTURE *src;
    unsigned int i;

    if (!tex)
        return 0;
    if (!tex->src)
        src = tex;
    else {
        src = tex->src;
        tex->src = 0;
        if (tex->group != -1 && tex->frames > 0 && group) {
            for (i = 0; i < d3_TexCount; i++) {
                if (d3_Textures[i].refs > 0 && d3_Textures[i].group == tex->group)
                    tex_ReleaseAlias(&d3_Textures[i], 0);
            }
        }
        tex_Release(tex);
    }
    return src;
}
