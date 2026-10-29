/* 0x42ecf0, upper-cases a name unless vfs_IgnoreCase is set. */

char *vfs_UpperCase(char *path)
{
    if (vfs_IgnoreCase)
        return path;
    return strupr(path);
}
