/* 0x45ff70, finds or creates a colour table holding the n colours of pal; map receives their indices */

unsigned short *ts_GetHiColTab(unsigned char *pal, unsigned int n, unsigned char *map)
{
    TSHICOL *h = 0;
    unsigned char *p;
    unsigned int i, j, k, found;

    if (ts_HiColCount > 0) {
        if (n > ts_HiCol[ts_HiColCount].free && ts_HiColCount == ts_MaxHiCol)
            return 0;
        for (i = 0; i < ts_HiColCount; i++) {
            found = 0;
            for (j = 0; j < n; j++) {
                for (k = 0; k < 0x100 - ts_HiCol[i].free; k++) {
                    if (ts_HiCol[i].rgb[k][0] == pal[j * 3] && ts_HiCol[i].rgb[k][1] == pal[j * 3 + 1] && ts_HiCol[i].rgb[k][2] == pal[j * 3 + 2])
                        found++;
                }
            }
            if (n - found <= ts_HiCol[i].free) {
                h = &ts_HiCol[i];
                break;
            }
        }
    }
    if (!h) {
        ts_HiCol[ts_HiColCount].tab = mem_Alloc(0x8200, "d3_ts:HiColTab");
        ts_HiCol[ts_HiColCount].free = 0x100;
        h = &ts_HiCol[ts_HiColCount];
        if (n < 0x100)
            ts_AddColor(0, 0, 0, h);
        ts_HiColCount++;
    }
    for (i = 0, p = pal; i < n; i++, p += 3)
        map[i] = ts_AddColor(p[0], p[1], p[2], h);
    return h->tab;
}
