/* 0x42e360, reads the texts of an already known file again from its .res file. */

int txt_ReloadTextFile(char *name)
{
    char path[0x100];
    void *f;
    int fi, i, n, size;
    int *offs;

    fi = txt_FindFile(name);
    if (fi == -1)
        return 0;
    sprintf(path, "textbin_%s\\%s.res", "german", txt_Files[fi].name);
    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (!f)
        return 0;
    vfs_fseek(f, 0, SEEK_SET);
    vfs_fread(&n, 4, 1, f);
    vfs_fread(&txt_Files[fi].first, 4, 1, f);
    vfs_fread(&txt_Files[fi].last, 4, 1, f);
    offs = mem_Alloc(n * 4, "txt_txt:txt_ReloadTextFile:PointerTable");
    vfs_fread(offs, n * 4, 1, f);
    for (i = 0; i < n; i++)
        vfs_fread(txt_Keys[txt_Files[fi].first + i], 0x50, 1, f);
    for (i = 0; i < n; i++)
        vfs_fread(&txt_Random[txt_Files[fi].first + i], 1, 1, f);
    vfs_fread(&size, 4, 1, f);
    txt_Files[fi].buf = mem_Alloc(size, "txt_txt:txt_ReloadTextFile:Pointer");
    vfs_fread(txt_Files[fi].buf, size, 1, f);
    for (i = 0; i < n; i++)
        txt_Ptrs[txt_Files[fi].first + i] = txt_Files[fi].buf + offs[i];
    vfs_fclose(f);
    mem_Free(offs);
    return 1;
}
