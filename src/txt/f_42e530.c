/* 0x42e530, loads a compiled text file (textbin_german\name.res) into a free slot. */

int txt_LoadTextFile(char *name)
{
    char msg[0x100];
    char path[0x104];
    void *f;
    int fi, i = 0, n = 0, size = 0;
    int *offs;

    for (fi = 0; fi < 0x80; fi++)
        if (!txt_Files[fi].name[0])
            break;
    if (fi >= 0x80)
        return 0;
    sprintf(path, "textbin_%s\\%s.res", "german", name);
    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (!f) {
        sprintf(msg, "Could not open textfile:%s", path);
        err_Message(msg);
        return 0;
    }
    vfs_fseek(f, 0, SEEK_SET);
    strcpy(txt_Files[fi].name, name);
    vfs_fread(&n, 4, 1, f);
    vfs_fread(&txt_Files[fi].first, 4, 1, f);
    vfs_fread(&txt_Files[fi].last, 4, 1, f);
    offs = mem_Alloc(n * 4, "txt_txt:txt_loadTextFile:PointerTable");
    vfs_fread(offs, 4, n, f);
    for (i = 0; i < n; i++)
        vfs_fread(txt_Keys[txt_Files[fi].first + i], 0x50, 1, f);
    for (i = 0; i < n; i++)
        vfs_fread(&txt_Random[txt_Files[fi].first + i], 1, 1, f);
    vfs_fread(&size, 4, 1, f);
    txt_Files[fi].buf = mem_Alloc(size, "txt_txt:txt_loadTextFile:Pointer");
    vfs_fread(txt_Files[fi].buf, size, 1, f);
    for (i = 0; i < n; i++)
        txt_Ptrs[txt_Files[fi].first + i] = txt_Files[fi].buf + offs[i];
    if (txt_Files[fi].last > txt_Count + 1)
        txt_Count = txt_Files[fi].last + 1;
    vfs_fclose(f);
    mem_Free(offs);
    return 1;
}
