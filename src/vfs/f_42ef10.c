/* 0x42ef10, adds a file to a directory: to the node list while it is being filled, else into the sorted
   entries. Of two files with the same name the newer one (or the one with the lower extension index) wins.
   @deep behaviour: 41 code hunk lines are register roles (the new entry pointer e and the name copy in eax vs ecx, add eax,ecx vs add ecx,eax, the shared tail's store base eax vs ecx) plus 2 nop fill bytes at the end; every store, load and constant pair agrees, no statement is missing or extra (asmdiff; notes/units/vfs.md round 10 recorded the same one-register difference for e). Register allocation - deep */

void vfs_AddFile(char *name, VFSDIR *d, VFSDIR *zip, uLong pos, uLong num, unsigned int ext, uLong date)
{
    char msg[0x400];
    char path[0x400];
/* */    VFSNODE *prev = 0;
    VFSNODE *n;
    VFSDIR *dir;
    VFSENTRY *e, *entries, *old, *dst;
    int lo, hi, mid, len, i, off, cnt;
    char *s, *src;

    if (d->sorted) {
        e = vfs_FindEntry(name, d, &dir, 0);
        if (!e) {
            if (d->entries) {
                lo = 0;
                hi = d->nentries - 1;
                while (hi >= lo) {
                    mid = (lo + hi) >> 1;
                    if (vfs_strcmp(name, d->entries[mid].name) > 0)
                        lo = mid + 1;
                    else
                        hi = mid - 1;
                }
                if (mid >= d->nentries)
                    mid = d->nentries;
                else if (vfs_strcmp(name, d->entries[mid].name) > 0)
                    mid++;
                len = strlen(name) + 1;
                for (i = 0; i < d->nentries; i++)
                    len += strlen(d->entries[i].name) + 1;
                entries = mem_Alloc(len + (d->nentries + 1) * sizeof(VFSENTRY), "vfs:AddFile(sorted, insert)");
                s = (char *)(entries + d->nentries + 1);
                for (lo = 0; lo < d->nentries; lo++) {
                    strcpy(s, d->entries[lo].name);
                    d->entries[lo].name = s;
                    s += strlen(d->entries[lo].name) + 1;
                }
                off = mid * sizeof(VFSENTRY);
                cnt = off;
                old = d->entries;
                VFS_COPY(entries, old, cnt);
                hi = d->nentries * sizeof(VFSENTRY) - off;
                src = (char *)d->entries + off;
                dst = entries + mid + 1;
                VFS_COPY(dst, src, hi);
                mem_Free(d->entries);
                d->entries = entries;
                e = (VFSENTRY *)((char *)entries + off);
            } else {
                len = strlen(name) + 1;
                d->nentries = 0;
                e = d->entries = mem_Alloc(len + sizeof(VFSENTRY), "vfs:AddFile(sorted, first)");
                s = (char *)(e + 1);
            }
            strcpy(s, name);
            e->name = s;
            e->date = date;
            e->zip = zip;
            e->pos = pos;
            e->num = num;
            e->ext = ext;
            d->nentries++;
        } else if (date > e->date || date == -1 || ext < (unsigned int)e->ext) {
            e->date = date;
            e->ext = ext;
            e->zip = zip;
            e->pos = pos;
            e->num = num;
        } else if (date == e->date && !zip) {
            e->zip = 0;
            e->pos = 0;
            e->num = 0;
            e->ext = 0;
        }
    } else {
        for (n = d->files; n; prev = n, n = n->next) {
            if (!vfs_strcmp(name, n->name)) {
                char mine[0x400] = "";
                char other[0x400] = "";
                vfs_DirPath(d, vfs_Root, path);
                if (n->zip) {
                    vfs_DirPath(n->zip, vfs_Root, mine);
                    *strrchr(mine, '/') = 0;
                    strcat(mine, vfs_Ext[n->ext]);
                    strcat(mine, " ");
                }
                if (zip) {
                    vfs_DirPath(zip, vfs_Root, other);
                    *strrchr(other, '/') = 0;
                    strcat(other, vfs_Ext[ext]);
                }
                sprintf(msg, "vfs: name collision: \"%s%s\" %s%s", path, name, mine, other);
                if (date > n->date || date == -1 || ext < (unsigned int)n->ext) {
                    n->date = date;
                    n->zip = zip;
                    n->pos = pos;
                    n->num = num;
                    n->ext = ext;
                } else if (date == n->date && !zip) {
                    n->zip = 0;
                    n->pos = 0;
                    n->num = 0;
                    n->ext = 0;
                }
                return;
            }
        }
        if (!d->files) {
            n = pool_Alloc(&vfs_EntryPool, sizeof(VFSNODE), 128);
            d->files = n;
        } else {
            n = pool_Alloc(&vfs_EntryPool, sizeof(VFSNODE), 128);
            prev->next = n;
        }
        strncpy(n->name, name, 0xff);
        n->zip = zip;
        n->pos = pos;
        n->num = num;
        n->ext = ext;
        n->date = date;
        n->next = 0;
    }
}
