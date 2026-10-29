/* 0x4f8ae0, the statistics of the characters and buildings into out[10] (floats): counts the characters of type 0 / 1 (+2) with an id (+0) and their two objects (+0x16c / +0x170) and adds 1.0 to the grid cell
   (+0x14 of the 8 x 8 grid 0x91ae60) of each building object; sums the 0x100 building records (kind below 0x44, +0x27 != -1) with the kind table 0x617468 (four shorts per kind) and the building kind bytes +0x247 / +0x248;
   then normalises the grid cells and rates the result (0x4f9d40, 0x4f9ce0, 0x4f9ec0). out[4] the number of characters, out[5] .. out[9] derived ratings. ROUGH
   @deep far draft (449 code / 8 slots / 4 fill hunks, 690 of 880 bytes; ROUGH from the layout session). Read against tools/asmdiff 0x4f8ae0 and the source: the original's frame is 0x64 (ours 0x60) and it walks the character records with a pointer into the record (esi = c + 0x16c, id at [esi-0x16c], kind at [esi-0x16a]) where ours reloads ai_Chars[i] fields; the statement reading (the 0x300 character loop with the kind 0/1 and id guards, the two object pointers +0x16c / +0x170, n0 / n1, the ai_BTypes kind guard, the grid +0x14 buildingCount += 1.0f; the 0x100 AIBLD loop (type < 0x44, owner != -1) with the kind table ORD_KIND, sumA / sumC / sumD, s58 / s54 / s50 / s4c from the kind bytes +0x247 / +0x248; the ORD_KIND(0x44)[0] * 2 floor; out[7] / out[4] / out[8] / out[5] with the float products; *out = gm_IncomeRating(); out[3] / out[1]; the 64-cell normalisation loop with the max m, the >= 1.0f clamp w, the 1/(cnt+cnt) * buildingCount and the sum; then ord_WeightedRecordSum / ord_StatisticsRating / out[2] and the out[9] tail) was not verified hunk by hunk against the disassembly, so the ROUGH marker stays (Q5) and the float / double spellings of out[5] / out[9] are unchecked - that is the first lead for the next session. Q4 left: 14 raw accesses (the character id / kind / object pointers, the object's +0x61, the ai_BTypes kind byte, the AIBLD type / owner, the grid +0xc / +0x10 / +0x14 through the char* ORD_GRID macro); the typed rewrite is blocked by the same missing reading (a wrong field role would change the meaning) and gv.h cannot include ai.h (ai_Time / ai_Chars / ai_BTypes conflict), so AIBLD / AIBTYPE would need the ORDBLD-style local views first - base work for the next session. Done here: Q1 ord_ComputeStatistics, Q2 clean (gm_IncomeRating folded with f3gm.c's name, ord_WeightedRecordSum, ai_Blds folded with ai.h's name for 0x889f74). */

#define ORD_GRID(c) ((char *)ord_GridCells + (c) * 0x18)
#define ORD_KIND(k) ((unsigned short *)(ord_KindTable + (k) * 8))
float gm_IncomeRating();                       /* 0x4f9ec0 */
void cm_SendMessage41(void *p);                         /* 0x4c5800 */

void ord_ComputeStatistics(float *out)
{
    int n1, n0, cnt, x, y, i, j, k;
    int sumA, sumC, sumD, s58, s54, s50, s4c;
    float f, g, lim, m, sum, w;
    char *c;
    char *o;
    char *p;
    char *b;
    unsigned char kind;
    int rec[12];
    int *ptr[2];
    unsigned short *t;

    n1 = 0;
    n0 = 0;
    cnt = 0;
    sumA = 0;
    sumC = 0;
    sumD = 0;
    s58 = s54 = s50 = s4c = 0;
    ord_RebuildGridSums();
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c == -1 || (c[2] != 0 && c[2] != 1))
            continue;
        cnt++;
        for (k = 0; k < 2; k++) {
            o = *(char **)(c + 0x16c + k * 4);
            if (o == 0)
                continue;
            if (k == 0)
                n0++;
            else
                n1++;
            if (*(int *)(o + 0x61) != 0) {
                kind = *(unsigned char *)(ai_BTypes + (unsigned char)o[0] * 0x24d);
                if ((kind < 0xb || kind > 0xd) && kind != 0x10) {
                    if (ord_GetGridCell((char *)(*(int *)(o + 0x61) + 0x4c), &x, &y) != 0)
                        ORD_CELLS[y + x * 8].buildingCount += 1.0f;
                }
            }
        }
    }
    p = ai_Blds;
    for (i = 0; i < 0x100; i++, p += 0xa9) {
        if (*p == 0 || (unsigned char)*p >= 0x44 || *(short *)(p + 0x27) == -1)
            continue;
        t = ORD_KIND((unsigned char)*p);
        b = ai_BTypes + (unsigned char)*p * 0x24d;
        sumA += t[0];
        sumC += t[2];
        sumD += t[1];
        if (t[1] != 0) {
            s58 += (unsigned char)b[0x247];
            s54 += (unsigned char)b[0x248];
        }
        if (t[3] != 0) {
            s50 += (unsigned char)b[0x247];
            s4c += (unsigned char)b[0x248];
        }
    }
    k = ORD_KIND(0x44)[0] * 2;
    if (sumA <= k)
        sumA = k;
    out[7] = (float)sumC;
    out[4] = (float)cnt;
    out[8] = (float)sumA;
    out[5] = ((float)cnt / (float)sumA) * (((float)s50 / (float)s4c) * ((float)s58 / (float)s54) + 0.5f) * (float)sumD;
    *out = gm_IncomeRating();
    out[3] = 1.0f - (float)n1 / (float)cnt;
    out[1] = 1.0f - (float)n0 / (float)cnt;
    m = 0;
    sum = 0;
    for (i = 0; i < 64; i++) {
        c = ORD_GRID(i);
        if (m <= *(float *)(c + 0xc))
            m = *(float *)(c + 0xc);
        w = *(float *)(c + 0x10) <= 1.0f ? 1.0f : *(float *)(c + 0x10);
        *(float *)(c + 0x10) = w;
        g = (1.0f / (float)(cnt + cnt)) * *(float *)(c + 0x14);
        *(float *)(c + 0x14) = g;
        sum = (*(float *)(c + 0xc) / w) * g + sum;
    }
    f = ord_WeightedRecordSum(rec);
    lim = ord_StatisticsRating();
    out[2] = (float)((1.0 - (f * 0.7 + lim * 0.3)) * sum);
    g = (float)(1.0 - out[2]);
    w = g * *out * (1.0f - out[1]) * (1.0f - out[3]);
    out[9] = (float)((w + w + (g + *out + (1.0f - out[1]) + (1.0f - out[3])) * 0.25) * 0.33333334f);
}
