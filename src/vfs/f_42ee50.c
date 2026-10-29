/* 0x42ee50, the same ignoring case. */

int vfs_CmpNameI(const char *name, const VFSENTRY *e)
{
    return stricmp(name, e->name);
}
