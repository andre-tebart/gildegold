/* 0x4f3b40, as 0x4f3a30 for the n record ids listed in ids[] (the records that exist in 0x91f700); returns how many were retired */

int unk_Unk4f3b40(int p, int n, int *ids)
{
    int cnt, m, i, k, j, id, t;
    char *c;
    int flag[8];
    int *who[8];
    char *q;
    int *r;

    cnt = 0;
    i = 0;
    c = gm_FindChar(p);
    m = 0;
    if (c != 0) {
        q = &ai_Chars[0][2];
        do {
            if (m > 7)
                break;
            if ((*q == 6 || *q == 7) && *(int *)(q + 2) != p) {
                flag[m] = 0;
                who[m] = (int *)(q - 2);
                m++;
            }
            q += 0x21c;
        } while ((int)q < (unsigned)((unsigned)ai_Chars + 0x65402));
    }
    if (n >= 1) {
        do {
            id = ids[i];
            r = (int *)gv_Graveyard;
            while (*r != id) {
                r = (int *)((char *)r + 0x2d);
                if ((int)r > ((unsigned)&ai_Unk91f716 + 0x59e9))
                    goto next;
            }
            k = 0;
            do {
                j = 0;
                if (m > 0) {
                    t = *(int *)(unk_Tab91b6e0 + k);
                    do {
                        if (t == who[j][1] && *(int *)(unk_Tab91b6e0 + 4 + k) == id)
                            flag[j] = 1;
                        j++;
                    } while (j < m);
                }
                k += 8;
            } while (k < 0x4000);
            ai_Unk4c4e90(id, 1);
            cnt++;
        next:
            i++;
        } while (i < n);
    }
    i = 0;
    if (m > 0) {
        do {
            if (flag[i] != 0)
                ai_Unk5a9430(who[i], c);
            i++;
        } while (i < m);
    }
    return cnt;
}
