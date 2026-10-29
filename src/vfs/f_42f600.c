/* 0x42f600, the same ignoring case. */

int vfs_CmpEntryI(const VFSENTRY *a, const VFSENTRY *b)
{
    return stricmp(a->name, b->name);
}
