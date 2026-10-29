/* 0x4bd4a0, puts every building site (0x58f7a0's list) on the grid */

void rd_AddBauplaetze(G15ROAD *r)
{
    int list[0x100];
    int n, i;

    memset(list, 0, sizeof(list));
    n = ai_FindBauplaetze(0, list);
    if (n != 0) {
        for (i = 0; i < n; i++) {
            if (list[i] != 0)
                rd_AllBauplatzToSupermap(r, (char *)list[i]);
        }
    }
}
