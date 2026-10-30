/* 0x49fc90, decide callback: trade in the item group where the character's share of the workshops is lowest */

int ai_Unk49fc90(int a, G5CH *p, int f, AIARG *o1)
{
    G7MKT m[7];
    float score[7];

    unsigned int best[7];
    int all[7];
    int own[7];
    unsigned char *b;
    unsigned char *bt;
    unsigned int lo32;
    int hi32;
    int off, g, i, lo, hi, d;
    float v;

    if (a == 0 && f == 0 && amt_Unk4e94a0(p) == 1) {
        memset(own, 0, sizeof(own));
        memset(all, 0, sizeof(all));
        memset(best, 0, sizeof(best));
        for (off = 0; off < 0xa900; off += 0xa9) {
            b = (unsigned char *)ai_BldsT + off;
            if (b[0] != 0 && *(short *)(b + 0x27) != -1) {
                bt = (unsigned char *)ai_BTypesT + b[0] * 0x24d;
                lo32 = 0;
                hi32 = 0;
                if (bt[0] < 0x20)
                    lo32 = 1 << bt[0];
                else
                    hi32 = 1 << (bt[0] - 0x20);
                if ((lo32 & ((unsigned)&he_Events + 0x49640)) != 0 && (g = gm_BuildingJob(bt[0])) != 0 && g < 8) {
                    g--;
                    if (*(short *)(b + 0x27) == (short)p->id)
                        own[g] += bt[0x247];
                    all[g] += bt[0x247];
                    if (best[g] == 0)
                        best[g] = b[0];
                    else if (((unsigned char *)ai_BTypesT)[best[g] * 0x24d + 0x247] < bt[0x247])
                        best[g] = b[0];
                }
            }
        }
        hi = -1;
        lo = -1;
        for (i = 0; i < 7; i++) {
            score[i] = -1.0f;
            m[i].key = best[i];
            m[i].n = 0;
            if (all[i] > 14)
                all[i] = 15;
            if (own[i] > 14)
                own[i] = 15;
            if (all[i] > 0 && ai_Unk4a4680(&m[i]) != 0) {
                v = (16.0f - (float)own[i]) * 0.06666667f * (((float)all[i] + 1.0f - (float)own[i]) / (float)all[i]) * m[i].avg;
                score[i] = v;
                if (lo == -1 || v < score[lo])
                    lo = i;
                if (hi == -1 || score[hi] < v)
                    hi = i;
            }
        }
        if (lo != -1 && hi != -1 && lo != hi) {
            d = 0;
            if (m[lo].avg > 1.0f || own[lo] == 0) {
                if (m[hi].avg < 1.0f)
                    return 0;
                if (all[hi] <= own[hi] * 2 + 1)
                    return 0;
                d = -1;
            }
            if (m[hi].avg < 1.0f || all[hi] <= own[hi] * 2 + 1) {
                if (m[lo].avg <= 1.0f && own[lo] != 0) {
                    d = 1;
                    goto set;
                }
            } else {
                if (d == 0)
                    d = util_Random(4) - 1;
                if (d == -1 || d == 1) {
set:
                    o1->kind = 0x15;
                    o1->unk04 = lo + 1;
                    o1->unk10 = d;
                    return 0x36;
                }
            }
        }
    }
    return 0;
}
