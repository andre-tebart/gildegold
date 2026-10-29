/* 0x4a7ac0, picks each NPC's strategy (seat->unk2c) from its hand: 3 nothing yet, 2 go for the rank, 1 pairs,
   0 random */

void ai_Unk4a7ac0(void)
{
    unsigned int cnt[6];
    int s, j, i, pairs, first;
    unsigned int n, maxn, maxr;
    unsigned char b5;
    G10CARD *c, *q;
    unsigned int *pc;

    first = 0;
    for (s = 1; s < 3; s++) {
        maxr = 0;
        maxn = 0;
        pairs = 0;
        c = ai_Unkf83660;
        pc = cnt;
        do {
            n = 0;
            for (i = 4; i != 0; i--) {
                if (c->owner == s)
                    n++;
                c++;
            }
            if ((int)n > 1)
                pairs++;
            if ((int)maxn < (int)n)
                maxn = n;
            *pc++ = n;
        } while (c < &ai_Unkf83660[24]);
        j = 0;
        c = ai_Unkf83660;
        do {
            n = 0;
            q = c;
            for (i = 6; i != 0; i--) {
                if (q->owner == s)
                    n++;
                q += 4;
            }
            if ((int)maxr < (int)n) {
                maxr = n;
                first = j;
            }
            cnt[j] = n;
            c++;
            j++;
        } while (c < &ai_Unkf83660[4]);
        b5 = ai_Unkf83654->unkb5;
        if (b5 < 5 && pairs < 2 && (int)maxn < 2) {
            ai_Unkf83654->seat[s].unk2c = 3;
        } else if (b5 == maxr) {
            ai_Unkf83654->seat[s].unk2c = 2;
        } else {
            if (ai_Unkf83654->phase < 6 && (int)(b5 - maxr) < 2) {
                for (j = 0; j < 6; j++)
                    if (ai_Unkf83660[j * 4 + first].owner != s && ai_Unkf83660[j * 4 + first].shown == 0)
                        break;
                if (j == 6) {
                    ai_Unkf83654->seat[s].unk2c = 2;
                    continue;
                }
            }
            if (b5 == 5 && (int)maxn < 2)
                ai_Unkf83654->seat[s].unk2c = 0;
            else
                ai_Unkf83654->seat[s].unk2c = 1;
        }
    }
}
