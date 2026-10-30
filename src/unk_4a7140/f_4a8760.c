/* 0x4a8760, decide callback: character p (or the one of the ctx record, kind 2) picks a partner in its district
   (a character with flag 1 within 33 units, else one of a higher rank within 40..71), into m (kind 7) */

int ai_Unk4a8760(int a, unsigned char *p, int *ctx, int d, unsigned char *m)
{
    G10DIST info, info2;
    G10DREC rec[6];
    unsigned char *who[6];
    unsigned char *o;
    unsigned char k;
    int id, n, cnt, i, j;
    unsigned int r;

    i = 0;
    o = 0;
    if (ctx[0] == 1) {
        if (p[0x168] != 0)
            return 0;
        id = *(int *)(p + 4);
        k = p[0x166];
    } else {
        if (ctx[0] != 2)
            return 0;
        o = (unsigned char *)ctx[0x87];
        if (o == 0)
            return 0;
        k = o[0x166];
        id = *(int *)(o + 4);
        o = 0;
    }
    if (amt_GetKind(k, &info) != 0) {
        n = amt_GetColumnOffices(info.id, 6, rec);
        j = 0;
        cnt = 0;
        if (n > 0) {
            for (j = 0; j < n; j++) {
                if (rec[j].k == 2) {
                    cnt++;
                    if (rec[j].id == id)
                        return 0;
                }
            }
            if (cnt > 2)
                return 0;
        }
        if (n > 0) {
            for (i = 0; i < n; i++)
                who[i] = (unsigned char *)gm_FindChar(rec[i].id);
        }
        if (n < 6)
            memset(&who[n], 0, (6 - n) * sizeof(who[0]));
        r = util_RandomU(6);
        for (j = 6; j != 0; j--) {
            if (who[r] != 0 && id != *(int *)(who[r] + 4)
                && gm_Liking(*(short *)who[r], *(short *)p, 1) < 33.0f && rec[r].k == 1) {
                o = who[r];
                if (o != 0 && *(int *)(p + 0x5c) != *(int *)(o + 4)) {
                    m[0] = 7;
                    *(int *)(m + 4) = *(int *)(o + 4);
                    return 1;
                }
                break;
            }
            r = (int)(r + 1) % 6;
        }
        if (info.lvl != 3 && info.lvl != 6 && info.lvl != 9 && p[0x130] == 7) {
            r = util_RandomU(6);
            for (j = 6; j != 0; j--) {
                if (who[r] != 0 && id != *(int *)(who[r] + 4)) {
                    if (amt_GetKind(who[r][0x166], &info2) == 0)
                        return 0;
                    if (info.lvl < info2.lvl && rec[r].k == 1
                        && gm_Liking(*(short *)who[r], *(short *)p, 1) < (float)util_RandomU(0x20) + 40.0f) {
                        o = who[r];
                        break;
                    }
                }
                r = (int)(r + 1) % 6;
            }
            if (o != 0 && *(int *)(p + 0x5c) != *(int *)(o + 4)) {
                m[0] = 7;
                *(int *)(m + 4) = *(int *)(o + 4);
                return 1;
            }
        }
    }
    return 0;
}
