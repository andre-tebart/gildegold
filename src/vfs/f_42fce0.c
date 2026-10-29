/* 0x42fce0, adds a real directory (recursively) or an archive (path with one of the vfs_Ext extensions) to
   the directory tree below parent. With parent 0 it creates the root and finishes it. */

VFSDIR *vfs_AddDir(char *path, VFSDIR *parent)
{
    SYSTEMTIME st;
    FILETIME ft;
    char buf[0x100];
    char dir[0x100];
    WIN32_FIND_DATA fd;
    char name[0x100];
    char sub[0x100];
    VFSDIR *d = 0;
    HANDLE h;
    char *ext, *p;
    char found;
    int i, n;

    strcpy(dir, path);
    vfs_UpperCase(dir);
    if (!parent)
        vfs_ToSlashes(dir);
    ext = strrchr(dir, '.');
    if (((n = strlen(dir) - 1) <= 0 || dir[n] != '/' && dir[n] != '\\') && ext) {
        for (i = 0; i <= 6; i++)
            if (!vfs_strcmp(ext, vfs_Ext[i]))
                vfs_AddZip(dir, parent, i);
    } else {
        if (dir[strlen(dir) - 1] != '/')
            strcat(dir, "/");
        strcpy(buf, dir);
        strcat(buf, "*");
        strcpy(sub, buf);
        h = FindFirstFile(buf, &fd);
        if (h != INVALID_HANDLE_VALUE) {
            if (parent) {
                strcpy(buf, dir);
                if ((p = strrchr(buf, '/')) && (*p = 0, p = strrchr(buf, '/'))) {
                    strcpy(name, p + 1);
                    strcat(name, "/");
                } else
                    strcpy(name, "");
            } else
                strcpy(name, dir);
            d = vfs_IGetSubDir(name, parent);
            do {
                if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")) {
                    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        strcpy(buf, dir);
                        strcat(buf, fd.cFileName);
                        strcat(buf, "/");
                        vfs_AddDir(buf, d);
                    } else {
                        found = 0;
                        strcpy(buf, fd.cFileName);
                        vfs_UpperCase(buf);
                        ext = strrchr(buf, '.');
                        if (ext) {
                            for (i = 0; i <= 6; i++) {
                                if (!vfs_strcmp(ext, vfs_Ext[i])) {
                                    strcpy(sub, dir);
                                    strcat(sub, buf);
                                    vfs_AddDir(sub, d);
                                    found = 1;
                                }
                            }
                        }
                        if (!found) {
                            FileTimeToLocalFileTime(&fd.ftLastWriteTime, &ft);
                            FileTimeToSystemTime(&ft, &st);
                            vfs_AddFile(buf, d, 0, 0, 0, 0,
                                        ((((st.wMonth << 5 | st.wDay) << 5 | st.wHour) << 6 | st.wMinute) << 5)
                                        | (st.wYear - 60) << 25 | st.wSecond >> 1);
                        }
                    }
                }
            } while (FindNextFile(h, &fd));
            FindClose(h);
        }
    }
    if (!parent && d)
        vfs_FinishedFiles(d, 1);
    return d;
}
