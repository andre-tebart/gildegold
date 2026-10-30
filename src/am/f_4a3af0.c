/* 0x4a3af0, fills the ring 0xf83c20 with the characters near character p (district, family, same group) */

/* one district's characters into the ring: the list of district k, entries passing cond, with flags f */
#define G7_ADD(k, f, cond)                                                          \
    n = amt_GetColumnOfficesOwned((k), 6, &info);                                                \
    for (j = 0; j < n; j++) {                                                       \
        if (r > ai_Unkf83c20 + 31)                                                  \
            break;                                                                  \
        if ((cond) && (o = (G5CH *)gm_FindChar(info.e[j].id)) != 0 && o != p) {    \
            r->flags = (f);                                                         \
            r->dist = gm_Liking(o->id, p->id, 1);                                \
            r->who = o;                                                             \
            cnt++;                                                                  \
            r++;                                                                    \
        }                                                                           \
    }

void ai_FillNearRing(AICHAR *c)
{
    G5CH *p = (G5CH *)c;
    struct { int unk00; struct { int id; unsigned char k; char pad[19]; } e[6]; } info;
    int kind[2];
    int dist[3];
    int k2, k3, k4;
    unsigned char k1;
    int cnt, n, j, i, g, step;
    unsigned int f;
    G5CH *o;
    AIPER *fam[6];
    G6RING *r;
    int *sz;

    cnt = 0;
    ai_Unk6f070c = 0;
    memset(ai_Unkf83c20, 0, 32 * sizeof(G6RING));
    r = ai_Unkf83c20;
    if (((unsigned char *)p)[0x166] == 0) {
        G7_ADD(1, 0x40, amt_GetKindLevel(info.e[j].k) < 3)
        G7_ADD(2, 0x40, amt_GetKindLevel(info.e[j].k) < 3)
        G7_ADD(3, 0x40, amt_GetKindLevel(info.e[j].k) < 3)
    } else {
        k1 = 0;
        k2 = 0;
        k3 = 0;
        k4 = 0;
        dist[0] = 0;
        dist[1] = 0;
        dist[2] = 0;
        g = amt_GetKindLevel(((unsigned char *)p)[0x166]);
        amt_GetKindB(((unsigned char *)p)[0x166], kind);
        if (kind[1] >= 1 && kind[1] <= 3) {
            k2 = 3;
            if (kind[1] == 1) {
                k1 = 2;
            } else {
                k1 = 1;
                if (kind[1] != 2)
                    k2 = 2;
            }
            if (g == 3) {
                k3 = 4;
                k4 = 5;
            }
        } else if (kind[1] >= 4 && kind[1] <= 5) {
            k1 = (kind[1] == 4) + 4;
            if (g == 6) {
                k3 = 6;
            } else if (g == 4) {
                dist[0] = 1;
                dist[1] = 2;
                dist[2] = 3;
            }
        } else if (g == 7) {
            dist[0] = 4;
            dist[1] = 5;
        }
        G7_ADD(kind[1], 0x10, 1)
        G7_ADD(k1, 1, 1)
        G7_ADD(k2, 1, 1)
        if (g == 3 || g == 6) {
            G7_ADD(k3, 0x40, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
            G7_ADD(k4, 0x40, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
        }
        if (g == 4 || g == 7) {
            G7_ADD(dist[0], 0x80, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
            G7_ADD(dist[1], 0x80, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
            G7_ADD(dist[2], 0x80, abs(amt_GetKindLevel(info.e[j].k) - g) < 2)
        }
    }
    if (p->unk168 != 0) {
        n = amt_GetOfficeApplicants((char)p->unk168, 6, fam);
        for (j = 0; j < n; j++) {
            if ((G5CH *)fam[j] != p) {
                for (i = 0; i < cnt; i++) {
                    if (ai_Unkf83c20[i].who == (G5CH *)fam[j]) {
                        ai_Unkf83c20[i].flags |= 2;
                        break;
                    }
                }
                if (i < cnt)
                    continue;
                if (r <= ai_Unkf83c20 + 31) {
                    r->flags = 2;
                    r->dist = gm_Liking(o->id, p->id, 1);
                    cnt++;
                    r->who = (G5CH *)fam[j];
                    r++;
                }
            }
        }
    }
    if (p->unk168 != 0 || ((unsigned char *)p)[2] == 5) {
        for (o = G5_CHARS; o < G5_CHARS + 768; o++) {
            f = 0;
            if (o != p && o->id != 0xffff) {
                k1 = ((unsigned char *)o)[2];
                if ((k1 == 6 || k1 == 7 || k1 == 5) && ((unsigned char *)p)[2] == 5)
                    f = 1;
                if (((unsigned char *)o)[0x168] == p->unk168)
                    f |= 4;
                if (f != 0) {
                    for (i = 0; i < cnt; i++) {
                        if (ai_Unkf83c20[i].who == o) {
                            ai_Unkf83c20[i].flags |= f;
                            break;
                        }
                    }
                    if (i < cnt)
                        continue;
                    if (r <= ai_Unkf83c20 + 31) {
                        r->flags = f;
                        r->dist = gm_Liking(o->id, p->id, 1);
                        r->who = o;
                        cnt++;
                        r++;
                    }
                }
            }
        }
    }
    step = (4 - ai_Difficulty) * 5;
    for (i = 0; i < cnt; i++) {
        k1 = ((unsigned char *)ai_Unkf83c20[i].who)[2];
        if (k1 == 6 || k1 == 7)
            ai_Unkf83c20[i].dist = (float)step + ai_Unkf83c20[i].dist;
    }
    ai_Unk6f070c = cnt;
    if (cnt < 4) {
        ai_Unk60034c = ai_Unkf83c20;
        ai_Unk600350 = 1;
        return;
    }
    g = 11;
    for (sz = &ai_Unk5ed1a8; g != 0; sz--, g--) {
        if (*sz < cnt)
            break;
    }
    ai_Unk60034c = &ai_Unkf83c20[util_RandomU(cnt)];
    ai_Unk600350 = ai_Unk5ed180[util_RandomU(g)];
    if (ai_Unk600350 > 1 && ai_Unk6f070c % ai_Unk600350 == 0)
        ai_Unk600350 += (util_RandomU(2) != 0 ? 2 : 0) - 1;
}
