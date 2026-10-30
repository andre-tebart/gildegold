/* 0x519bf0, the object list ai_Objects (0x5008 records of 0x43 bytes; the used ones have a nonzero first word): the count, then each used record in pieces (2, 4, 4, 4, 4, 1, 1 bytes and 0x1f bytes at +0x1c) */


int ls_WriteObjects(int f)
{
    int n = 0;
    int i, off;
    short *p = ai_Objects;
    short *q;
    char *r;

    for (i = 0x5008; i != 0; i--) {
        if (*p != 0)
            n++;
        p = (short *)((char *)p + 0x43);
    }
    if (vfs_fwrite(&n, 4, 1, f) <= 0)
        return 0;
    off = 0;
    do {
        r = (char *)(off + (int)ai_Objects);
        q = (short *)(off + (int)ai_Objects);
        if (*q != 0) {
            if (vfs_fwrite(r, 2, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 2, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 6, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 10, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0xe, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x12, 1, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x13, 1, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x1c, 0x1f, 1, f) <= 0)
                return 0;
        }
        off += 0x43;
    } while (off < 0x14f218);
    return 1;
}
