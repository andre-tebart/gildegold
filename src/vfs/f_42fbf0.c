/* 0x42fbf0, builds the path of directory d relative to the directory stop. */

char *vfs_DirPath(VFSDIR *d, VFSDIR *stop, char *out)
{
    char tmp[0x100];
    VFSDIR *p = d;

    if (!p)
        return 0;
    strcpy(out, p->name);
    do {
        if (p->parent == stop)
            break;
        p = p->parent;
        strcpy(tmp, p->name);
        strcat(tmp, out);
        strcpy(out, tmp);
    } while (p);
    return out;
}
