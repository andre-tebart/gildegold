/* 0x430d90, reads one byte through vfs_fread; returns it as 0..255, or -1 at the end of the file */

int vfs_fgetc(VFSFILE *f)
{
    char c;

    if (vfs_fread(&c, 1, 1, f) == 1)
        return c;
    return -1;
}
