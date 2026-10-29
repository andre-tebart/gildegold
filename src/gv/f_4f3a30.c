/* 0x4f3a30, retires up to n records of the character p's graveyard state-1 entries (0x4c4e90 (id, 1) each): the other player characters (kind 6 / 7, up to 8, flag per slot) whose pair (their id, the record id) exists in 0x91b6e0 are
   told (0x5a9430 (them, p's character)); returns how many records were retired. ROUGH */

int unk_Unk4f3a30(int p, int n)
{
    int cnt, off, k, j, m, i;
    char *c;
    int flag[8];
    int *who[8];
    char *q;
    int *t;

    cnt = 0;
    off = 0;
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
    do {
        if (n <= cnt)
            break;
        if (*(int *)(gv_Graveyard + 0x16 + off) == p && *(int *)(gv_Graveyard + 0x25 + off) == 1) {
            k = 0;
            do {
                j = 0;
                if (m > 0) {
                    i = *(int *)(unk_Tab91b6e0 + k);
                    do {
                        if (i == who[j][1] && *(int *)(unk_Tab91b6e0 + 4 + k) == *(int *)(gv_Graveyard + off))
                            flag[j] = 1;
                        j++;
                    } while (j < m);
                }
                k += 8;
            } while (k < 0x4000);
            ai_Unk4c4e90(*(int *)(gv_Graveyard + off), 1);
            cnt++;
        }
        off += 0x2d;
    } while (off < 0x5a00);
    j = 0;
    if (m > 0) {
        do {
            if (flag[j] != 0)
                ai_Unk5a9430(who[j], c);
            j++;
        } while (j < m);
    }
    return cnt;
}
