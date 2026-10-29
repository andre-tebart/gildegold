/* 0x42f620, moves the file nodes of a directory into one sorted array of entries (names stored behind the
   records), and with recurse also of its subdirectories and following siblings. */

void vfs_FinishedFiles(VFSDIR *d, char recurse)
{
    VFSNODE *n, *next;
    char *s;
    int count, len, i;

    if (d->files && !d->sorted) {
        len = 0;
        count = 0;
        for (n = d->files; n; n = n->next) {
            count++;
            len += strlen(n->name) + 1;
        }
        d->nentries = count;
        d->entries = mem_Alloc(len + count * sizeof(VFSENTRY), "vfs:FinishedFiles");
        s = (char *)(d->entries + d->nentries);
        if (n = d->files) {
            i = 0;
            do {
                strcpy(s, n->name);
                d->entries[i].name = s;
                d->entries[i].date = n->date;
                d->entries[i].zip = n->zip;
                d->entries[i].pos = n->pos;
                d->entries[i].num = n->num;
                d->entries[i].ext = n->ext;
                i++;
                s += strlen(n->name) + 1;
                next = n->next;
                pool_Free(&vfs_EntryPool, n);
            } while (n = next);
        }
        d->files = 0;
        if (vfs_IgnoreCase)
            qsort(d->entries, d->nentries, sizeof(VFSENTRY), (int (*)(const void *, const void *))vfs_CmpEntryI);
        else
            qsort(d->entries, d->nentries, sizeof(VFSENTRY), (int (*)(const void *, const void *))vfs_CmpEntry);
    } else if (!d->files)
        d->nentries = 0;
    d->sorted = 1;
    if (recurse) {
        if (d->child)
            vfs_FinishedFiles(d->child, 1);
        if (d->next)
            vfs_FinishedFiles(d->next, 1);
    }
}
