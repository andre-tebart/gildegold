/* 0x442080, loads the source textures (3 mip levels) of tile type idx (< 0: the first free one) */

int d3fl_LoadTexture(D3FLMAP *map, char *name, int idx)
{
    char buf[256];
    char *path;
    int i, k;

    if (idx < 0)
        for (k = 0; map->typenames[k][0]; k++)
            ;
    else
        k = idx;
    if (k < 8) {
        map->typenames[k][0] = d3fl_EmptyName;
        for (i = 0; i < 3; i++)
            map->src[i][k] = 0;
        if (tex_FindFile(name)) {
            strncpy(map->typenames[k], name, 0x40);
            for (i = 0; i < 3; i++) {
                strcpy(buf, name);
                strcat(buf, d3fl_Suffix[i]);
                if (path = tex_FindFile(buf)) {
                    map->src[i][k] = mem_Alloc(0x4000, "d3_fl:fltx_SourceData");
                    bmp_Load(path, map->src[i][k], 0, 0x11);
                } else
                    strcat(buf, " not found (FloorTexture)!");
            }
            return k;
        }
        sprintf(buf, "Floor-Texture \"%s\" not found!", name);
    }
    return -1;
}
