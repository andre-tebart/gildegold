/* 0x4fe370, builds the building lookup tables: 0x875360 (the first building of the object classes 6 .. 16, fixed numbers) and 0x919f80 (for every object: the building of its class that follows from the kind runs: the first building of the class plus (the class length * the position of this building in its run of equal kinds / the run length)); objects without a building get the first building of their class. ROUGH */

void gm_InitBuildingTables(void)
{
    int run, len, i, j, k, w, u, base, cnt, v;
    char cur, kind;
    char *p, *q;
    unsigned short *wp;

    memset(ai_Unk875360, 0, 0x64);
    for (i = 0; i < 0x2dc; i++)
        ai_Unk919f80[i] = 0x58;
    run = 0;
    len = 0;
    ai_Unk875360[8] = 0x2f;
    ai_Unk875360[7] = 0x32;
    ai_Unk875360[6] = 0x29;
    ai_Unk875360[10] = 0x21;
    ai_Unk875360[14] = 0x1e;
    ai_Unk875360[15] = 0x1f;
    ai_Unk875360[16] = 0x20;
    ai_Unk875360[11] = 0x17;
    ai_Unk875360[12] = 0x35;
    ai_Unk875360[9] = 0x14;
    cur = 'X';
    p = ai_BTypes;
    for (i = 0; i < 0x58; i++, p += 0x24d) {
        kind = *p;
        if (kind == cur) {
            run++;
        } else {
            run = 0;
            len = 0;
            j = i;
            for (q = p; (cur = kind, j < 0x58 && *q == kind); q += 0x24d) {
                len++;
                j++;
            }
        }
        if (p[0x22] != 0) {
            wp = (unsigned short *)(p + 0x23);
            for (k = 0; k < (unsigned char)p[0x22]; k++, wp++) {
                if (*wp != 0xffff) {
                    u = *wp & 0x7fff;
                    base = ai_Unk875360[(unsigned char)ai_Goods[u * 0x41 + 0x21]];
                    q = ai_BTypes + base * 0x24d;
                    kind = *q;
                    cnt = 0;
                    for (j = base; j < 0x58 && *q == kind; q += 0x24d) {
                        cnt++;
                        j++;
                    }
                    v = base + (cnt * run) / len;
                    if (v < ai_Unk919f80[u])
                        ai_Unk919f80[u] = v;
                }
            }
        }
    }
    for (i = 0; i < 0x2dc; i++) {
        if (ai_Unk919f80[i] == 0x58)
            ai_Unk919f80[i] = ai_Unk875360[(unsigned char)ai_Goods[0x21 + i * 0x41]];
    }
}
