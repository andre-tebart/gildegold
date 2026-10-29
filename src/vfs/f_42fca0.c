/* 0x42fca0, the path of a file relative to stop, 0 if it is not found. */

char *vfs_GetPath(char *name, VFSDIR *d, VFSDIR *stop, char *out)
{
    VFSENTRY *e;

    e = vfs_FindFile(name, d, &d);
    if (e) {
        vfs_EntryPath(e, d, stop, out);
        return out;
    }
    return 0;
}
