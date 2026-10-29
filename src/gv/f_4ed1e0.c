/* 0x4ed1e0, writes the avatar table to the file f: the count 0x20, then per record the id (+0) and the used byte (+0xf2); 1 when all was written */

int gv_WriteAvatars(void *f)
{
    unsigned n, i;
    char *p;

    n = 0x20;
    if (f == 0)
        return 0;
    if (vfs_fwrite(&n, 4, 1, f) == 0)
        return 0;
    i = 0;
    if (n > 0) {
        p = &unk_Avatars[0][0xf2];
        do {
            if (vfs_fwrite(p - 0xf2, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(p, 1, 1, f) == 0)
                return 0;
            i++;
            p += 0xf4;
        } while (i < n);
    }
    return 1;
}
