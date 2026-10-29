/* 0x42f5c0, qsort comparator of two entries by name. */

int vfs_CmpEntry(const VFSENTRY *a, const VFSENTRY *b)
{
    return strcmp(a->name, b->name);
}
