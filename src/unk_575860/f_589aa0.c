/* 0x589aa0, loads the 12-byte slot table 0x6f5ec0 from a file (142 B): n = 0xb read through 0x430e30 (&n, 4, 1,
   fh), then each slot (the byte +0, the dword +4, the dword +8); 0 on a short read, 1 */

unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);

int ai_Unk589aa0(int fh)
{
    int n;
    int i;
    char *p;

    n = 0xb;
    if (vfs_fwrite(&n, 4, 1, fh) == 0)
        return 0;
    i = 0;
    p = (char *)(unsigned)&full_data_6f5ec4;
    while (i < n) {
        if (vfs_fwrite(p - 4, 1, 1, fh) == 0)
            return 0;
        if (vfs_fwrite(p, 4, 1, fh) == 0)
            return 0;
        if (vfs_fwrite(p + 4, 4, 1, fh) == 0)
            return 0;
        i++;
        p += 0xc;
    }
    return 1;
}
