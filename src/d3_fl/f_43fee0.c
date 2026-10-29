/* 0x43fee0, detail level (1, 2 or 4) of a tile, by its distance */

char d3fl_TileLod(D3FLMAP *map, D3FLTILE *t, float unused, int detail)
{
    char lod;
    char old;
    float d, k;

    if (!(lod = map->lod)) {
        if (!map->divide[0])
            return 0;
        d = t->centerDist / map->scale;
        k = (40.0f - detail) * 0.4f;
        if (d > (k + map->lodDist4) + t->lodDistBias)
            lod = 4;
        else if (d > (k + map->lodDist2) + t->lodDistBias)
            lod = 2;
        else
            lod = 1;
        old = t->lod;
        if (old && old != -1 && lod != old) {
            t->nextLod = lod;
            if (++t->lodDelay <= 8)
                return old;
        }
        t->lodDelay = 0;
    }
    return lod;
}
