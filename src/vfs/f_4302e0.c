/* 0x4302e0, creates the real directories of a path, one level at a time. */

char vfs_MakeDirs(char *name, int unk)
{
    char path[0x100];
    char tmp[0x100];
    char dir[0x100];
    char base[0x100];
    struct _finddata_t fd;
    char *p;
    long h;

    if (!name || !*name)
        return 0;
    strcpy(path, name);
    p = strrchr(path, '/');
    if (p) {
        vfs_DirPath((VFSDIR *)unk, 0, base);
        p[1] = 0;
        strcpy(tmp, path);
        p = strchr(tmp, '/');
        while (p) {
            p++;
            strcpy(path, p);
            *p = 0;
            strcat(base, tmp);
            strcpy(dir, base);
            p = strrchr(dir, '/');
            if (p)
                *p = 0;
            h = _findfirst(dir, &fd);
            if (h == -1) {
                if (_mkdir(dir))
                    return 0;
            } else
                _findclose(h);
            strcpy(tmp, path);
            p = strchr(tmp, '/');
        }
    }
    return 1;
}
