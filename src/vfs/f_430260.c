/* 0x430260, frees a directory list with its files and subdirectories. */

void vfs_FreeDir(VFSDIR *d)
{
    VFSDIR *next;
    VFSNODE *e, *enext;

    while (d) {
        next = d->next;
        for (e = d->files; e; e = enext) {
            enext = e->next;
            pool_Free(&vfs_EntryPool, e);
        }
        if (d->entries) {
            mem_Free(d->entries);
            d->entries = 0;
        }
        if (d->child)
            vfs_FreeDir(d->child);
        mem_Free(d);
        d = next;
    }
}
