/* 0x460130, a free slot: searched from the front for flags.b[0] == 0xff, else from the back */

TSTEXTURE *ts_AllocSlot(TSFLAGS flags, int *index)
{
    TSTEXTURE *tex;
    int i;

    if (ts_TexUsed == d3_TexCount) {
        if (index)
            *index = -1;
        return 0;
    }
    if (flags.b[0] >= 0xff && !(flags.b[2] & 1)) {
        for (i = 0, tex = d3_Textures; tex->refs > 0; i++, tex++)
            ;
    } else {
        for (i = d3_TexCount - 1, tex = &d3_Textures[i]; tex->refs > 0; i--, tex--)
            ;
    }
    if (index)
        *index = i;
    return tex;
}
