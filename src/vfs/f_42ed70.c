/* 0x42ed70, returns the subdirectory `name` of parent, creating it if needed. */

VFSDIR *vfs_IGetSubDir(char *name, VFSDIR *parent)
{
    VFSDIR *d, *p;

    if (parent)
        for (d = parent->child; d; d = d->next)
            if (!vfs_strcmp(name, d->name))
                return d;
    d = mem_Alloc(sizeof(VFSDIR), "vfs:IGetSubDir");
    strcpy(d->name, name);
    d->sorted = 0;
    d->parent = parent;
    if (parent) {
        p = parent->child;
        if (!p) {
            parent->child = d;
            return d;
        }
        while (p->next)
            p = p->next;
        p->next = d;
    }
    return d;
}
