/* 0x42f8f0, looks a file path up below d; *found gets the directory of the entry. A file name starting with
   '*' is searched in all subdirectories. */

VFSENTRY *vfs_FindFile(char *path, VFSDIR *d, VFSDIR **found)
{
    VFSDIR *dir = d;
    char buf[0x100];
    char name[0x100];
    VFSENTRY *e;
    char *p;

    strcpy(buf, path);
    vfs_UpperCase(buf);
    vfs_ToSlashes(buf);
    for (p = buf; *p && *p == '/'; p++)
        ;
    strcpy(name, p);
    if (p != buf)
        strcpy(buf, name);
    p = strrchr(buf, '/');
    if (p) {
        p++;
        strcpy(name, p);
        *p = 0;
        dir = vfs_FindDir(buf, dir);
        if (!dir)
            return 0;
    }
    if (name[0] == '*')
        e = vfs_FindEntry(name + 1, dir, &dir, 1);
    else
        e = vfs_FindEntry(name, dir, &dir, 0);
    if (e)
        *found = dir;
    return e;
}
