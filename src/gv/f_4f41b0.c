/* 0x4f41b0, for the character c: goes over the 512 records of 0x91f700 (0x2d bytes; int +0x25 != 0, int +0x16 = the accused character id != c's) whose pair (c's id, record id) is not yet in the table 0x91b6e0, counts them per accused
   character (up to 0x300), and returns the character with the most records; ids[] gets the ids (+0) of its records (up to max), *cnt their number; 0 when there is none. ROUGH */

char *gv_FindAccused(char *c, int max, int *ids, int *cnt)
{
    char *who[0x300];
    int count[0x300];
    char *rec[0x600];
    int i, j, n, nw, nr, best, bestn;
    char *o;
    int *q;

    *cnt = 0;
    memset(who, 0, sizeof(who));
    memset(count, 0, sizeof(count));
    nr = 0;
    n = 0;
    nw = 1;
    for (i = 0; i < 0x5a00; i += 0x2d) {
        if (*(int *)(gv_Graveyard + i + 0x25) == 0 || *(int *)(gv_Graveyard + i + 0x16) == *(int *)(c + 4))
            continue;
        o = gm_FindChar(*(int *)(gv_Graveyard + i + 0x16));
        if (o == 0)
            continue;
        for (q = (int *)(unk_Tab91b6e0 + 4); (int)q < ((unsigned)unk_Tab91b6e0 + 0x4004); q += 2)
            if (q[-1] == *(int *)(c + 4) && *q == *(int *)(gv_Graveyard + i))
                break;
        if ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004))
            continue;
        rec[nr++] = gv_Graveyard + i;
        for (j = 0; nw > 0 && j < 0x300; j++) {
            if (who[j] == 0) {
                who[j] = o;
                count[j]++;
                n = nw++;
                break;
            }
            if (*(int *)(who[j] + 4) == *(int *)(o + 4)) {
                count[j]++;
                break;
            }
            if (j + 1 >= nw)
                break;
        }
    }
    if (nr == 0 || n == 0)
        return 0;
    o = who[0];
    best = 0;
    bestn = 0;
    for (j = 0; j < n; j++) {
        if (count[j] > bestn) {
            o = who[j];
            bestn = count[j];
        }
    }
    j = 0;
    for (i = 0; i < nr && j < max; i++) {
        if (*(int *)(rec[i] + 0x16) == *(int *)(o + 4))
            ids[j++] = *(int *)rec[i];
    }
    *cnt = j;
    return o;
}
