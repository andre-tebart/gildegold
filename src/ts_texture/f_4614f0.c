/* 0x4614f0, a new slot sharing the surfaces of src (group: also for the other frames) */

TSTEXTURE *tex_CreateAlias(TSTEXTURE *src, TSFLAGS flags, char group)
{
    TSTEXTURE *a;
    unsigned int i;

    if (!src)
        return 0;
    a = ts_AllocSlot(flags, 0);
    if (a) {
        D3_COPY_AT(a, src, sizeof(TSTEXTURE));
        a->flags = flags;
        a->refs = 1;
        a->src = src;
        a->alias = 1;
        if (src->alias)
            a->mask = src->mask;
        else
            a->mask = src - d3_Textures;
        ts_TexUsed++;
        if (a->group != -1 && a->frames > 0 && group) {
            a->group = ts_NextGroup;
            for (i = 0; i < d3_TexCount; i++) {
                if (d3_Textures[i].refs > 0 && d3_Textures[i].group == src->group)
                    tex_CreateAlias(&d3_Textures[i], flags, 0);
            }
            ts_NextGroup++;
        }
    }
    return a;
}
