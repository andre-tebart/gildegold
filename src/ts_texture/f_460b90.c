/* 0x460b90, sets the current frame of the object's animated textures */

char tex_Animate(D3OBJECT *obj, unsigned int time)
{
    TSTEXTURE **texs;
    TSTEXTURE *tex, *f;
    TSTRI *tri;
    unsigned int seed;
    int i, k, n, ntris, group;

    if (obj->info && ((TSMESH *)obj->info)->unk10 && ((TSMESH *)obj->info)->nkeys > 0) {
        texs = ((TSMESH *)obj->info)->texs;
        seed = util_Hash(&obj, 4);
        ntris = ((TSMESH *)obj->info)->ntris;
        n = ((TSMESH *)obj->info)->unk10->ntex;
        for (i = 0; i < n; i++) {
            tex = texs[i];
            if (tex && tex->frames > 0 && !tex->frame && tex->speed > 0 && tex->refs > 0) {
                group = tex->group;
                f = tex_GetFrame(tex - d3_Textures, (time / d3_AnimDiv[tex->speed] + seed) % tex->frames);
                for (k = ntris, tri = ((TSMESH *)obj->info)->tris; k > 0; k--, tri++) {
                    if (tri->tex && tri->tex->group == group)
                        tri->tex = f;
                }
            }
        }
    }
    return 1;
}
