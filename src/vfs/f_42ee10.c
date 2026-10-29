/* 0x42ee10, bsearch comparator of a name with the name of an entry. */

int vfs_CmpName(const char *name, const VFSENTRY *e)
{
    return strcmp(name, e->name);
}
