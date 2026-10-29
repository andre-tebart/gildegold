/* 0x4a7e70, the open cards (2 .. unkb5-1) of seat s: the most common group (count, group) and rank (count, rank) */

void ai_Unk4a7e70(G10SEAT *s, int *n, int *g, int *m, int *r)
{
    int cnt[10];
    int i, k;
    int *p;

    cnt[1] = 0;
    cnt[5] = 0;
    cnt[2] = 0;
    cnt[6] = 0;
    cnt[3] = 0;
    cnt[4] = 0;
    cnt[7] = 0;
    cnt[8] = 0;
    cnt[0] = 0;
    cnt[9] = 0;
    if (ai_Unkf83654->unkb5 > 2) {
        i = ai_Unkf83654->unkb5 - 2;
        p = &s->card[2];
        do {
            k = *p++;
            cnt[ai_Unkf83660[k].rank]++;
            cnt[ai_Unkf83660[k].suit + 4]++;
        } while (--i);
    }
    *n = cnt[4];
    *g = 0;
    for (i = 1; i < 6; i++) {
        if (*n < cnt[i + 4]) {
            *n = cnt[i + 4];
            *g = i;
        }
    }
    *m = cnt[0];
    *r = 0;
    for (i = 1; i < 4; i++) {
        if (*m < cnt[i]) {
            *m = cnt[i];
            *r = i;
        }
    }
}
