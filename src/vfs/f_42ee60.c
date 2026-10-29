/* 0x42ee60, looks a file up in the sorted entries of d (and of its subdirectories if recurse). */

/* ---- 0x42ee60..0x430130 (worker vfs2) ---- */

VFSENTRY *vfs_FindEntry(char *name, VFSDIR *d, VFSDIR **found, char recurse)
{
    VFSENTRY *e;

    if (name && *name && d) {
        if (d->nentries > 0 && d->sorted) {
            e = bsearch(name, d->entries, d->nentries, sizeof(VFSENTRY),
                        (int (*)(const void *, const void *))(vfs_IgnoreCase ? vfs_CmpNameI : vfs_CmpName));
            if (e) {
                *found = d;
                return e;
            }
        }
        if (recurse)
            for (d = d->child; d; d = d->next)
                if (e = vfs_FindEntry(name, d, found, 1))
                    return e;
    }
    return 0;
}
