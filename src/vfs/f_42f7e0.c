/* 0x42f7e0, walks a directory path ("a/b/") down from d; 0 if a directory is missing. @deep behaviour: 26 code hunk lines are register roles and stack slots (dir ebx vs esi, the name copy edi/esi swapped, push edi moved, the string buffers at [esp+0xc/0x10] vs [esp+0x8/0xc] after the different push set), 64 regs hunk lines; the same calls (strcpy, vfs_strcmp/vfs_FindEntry in the loop) and constants agree, no statement differs (asmdiff, listing). Register/slot allocation - deep */

VFSDIR *vfs_FindDir(char *path, VFSDIR *d)
{
    char buf[0x100];
    char name[0x100];
    char *p;
    VFSDIR *dir = d;

    if (!path)
        return 0;
    if (*path) {
        strcpy(buf, path);
        vfs_UpperCase(buf);
        if (buf[strlen(buf) - 1] != '/')
            strcat(buf, "/");
        strcpy(name, buf);
        while ((p = strchr(name, '/')) && dir) {
            p++;
            strcpy(buf, p);
            *p = 0;
            for (dir = dir->child; dir && vfs_strcmp(dir->name, name); dir = dir->next)
                ;
            strcpy(name, buf);
        }
    }
    return dir;
}
