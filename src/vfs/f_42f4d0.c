/* 0x42f4d0, adds a file given by its path below d, creating the directories on the way. */

void vfs_AddFilePath(char *path, VFSDIR *d, VFSDIR *zip, uLong pos, uLong num, int ext, uLong date)
{
    char buf[0x100];
    char sub[0x100];
    char *p, *q;

    if (path[strlen(path) - 1] != '/') {
        VFSDIR *cur = d;
        strcpy(buf, path);
        p = buf;
        for (q = strchr(buf, '/'); q; q = strchr(p, '/')) {
            strcpy(sub, p);
            strchr(sub, '/')[1] = 0;
            cur = vfs_IGetSubDir(sub, cur);
            p = q + 1;
        }
        vfs_AddFile(p, cur, zip, pos, num, ext, date);
    }
}
