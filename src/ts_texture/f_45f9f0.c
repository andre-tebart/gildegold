/* 0x45f9f0 */

char *tex_FindFile(char *name)
{
    char path[0x100];

    strcpy(path, "*");
    strcat(path, name);
    strcat(path, ".BMP");
    return vfs_GetPath(path, ts_Palette, d3_Unk6333fc, d3_Unk633e00);
}
