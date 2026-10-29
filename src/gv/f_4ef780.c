/* 0x4ef780, 1 when the id a[0] is in the table (+0x10, 0x44 bytes per entry, gv_EntryCounts[b[0]] of them) of the entry b */

int gv_EntryHasId(int *a, int *b)
{
    int i, n;
    int *p;

    i = 0;
    n = gv_EntryCounts[b[0]];
    if (n > 0) {
        p = (int *)b[4];
        do {
            if (*p == a[0])
                return 1;
            i++;
            p += 0x11;
        } while (i < n);
    }
    return 0;
}
