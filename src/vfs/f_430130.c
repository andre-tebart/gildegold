/* 0x430130, adds the contents of a zip archive to the directory tree. */

void vfs_AddZip(char *path, VFSDIR *d, int ext)
{
    uLong pos, num;
    unz_file_info info;
    char name[0x100];
    unzFile zip;
    char *p;
    VFSDIR *r;

    zip = unzOpen(path);
    if (zip) {
        strcpy(name, path);
        *strrchr(name, '.') = 0;
        p = strrchr(name, '/');
        if (!p)
            p = name;
        else
            p++;
        strcat(p, "/");
        r = vfs_IGetSubDir(p, d);
        do {
            if (unzGetCurrentFileInfo(zip, &info, name, 0x100, 0, 0, 0, 0) == 0) {
                vfs_UpperCase(name);
                vfs_ToSlashes(name);
                if (info.uncompressed_size > 0) {
                    unzGetFilePos(zip, &pos, &num);
                    vfs_AddFilePath(name, r, r, pos, num, ext, info.dosDate);
                }
            }
        } while (unzGoToNextFile(zip) == 0);
        unzClose(zip);
    }
}
