/* 0x442280, reloads the source textures of all tile types (d3fl_ReloadTextures in d3.h) */

void d3fl_ReloadTextures(D3FLMAP *map)
{
    char buf[256];
    char *path;
    int i, j;

    if (map) {
        d3fl_Invalidate(map);
        for (i = 0; i < 8; i++)
            for (j = 0; j < 3; j++) {
                strcpy(buf, map->typenames[i]);
                strcat(buf, d3fl_Suffix[j]);
                if ((path = tex_FindFile(buf)) && map->src[j][i])
                    bmp_Load(path, map->src[j][i], 0, 0x11);
            }
    }
}
