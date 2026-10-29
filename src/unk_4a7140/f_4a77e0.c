/* 0x4a77e0, the NPCs exchange a card by their strategy (seat->unk2c): keep pairs (1), the most common rank (2) */

void ai_Unk4a77e0(void)
{
    int cnt[6];
    int s, g, j, i, n, k;
    int pairs, maxn, first, second, maxr, bestr;
    G10CARD *c, *q;
    G10SEAT *st;

    first = 0;
    second = 0;
    bestr = 0;
    k = -1;
    for (s = 1; s < 3; s++) {
        g = 0;
        maxn = 0;
        pairs = 0;
        maxr = 0;
        c = ai_Unkf83660;
        do {
            n = 0;
            for (i = 4; i != 0; i--) {
                if (c->owner == s)
                    n++;
                c++;
            }
            if (n > 1)
                pairs++;
            if (maxn < n) {
                second = first;
                maxn = n;
                first = g;
            }
            cnt[g] = n;
            g++;
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
            if (maxr < n) {
                maxr = n;
                bestr = j;
            }
            cnt[j] = n;
            c++;
            j++;
        } while (c < &ai_Unkf83660[4]);
        st = &ai_Unkf83654->seat[s];
        switch (st->unk2c) {
        case 0:
            k = util_Random(3);
            break;
        case 1:
            if (pairs == 1) {
                if (ai_Unkf83660[st->card[0]].suit != first)
                    k = 0;
                else if (ai_Unkf83660[st->card[1]].suit != first)
                    k = 1;
            } else if (pairs == 2) {
                if (ai_Unkf83660[st->card[0]].suit != first && ai_Unkf83660[st->card[0]].suit != second)
                    k = 0;
                else if (ai_Unkf83660[st->card[1]].suit != first && ai_Unkf83660[st->card[1]].suit != second)
                    k = 1;
            }
            break;
        case 2:
            if (ai_Unkf83660[st->card[0]].rank != bestr)
                k = 0;
            else if (ai_Unkf83660[st->card[1]].rank != bestr)
                k = 1;
            break;
        case 3:
            if (pairs == 1) {
                if (ai_Unkf83660[st->card[0]].suit == first)
                    k = 0;
                else if (ai_Unkf83660[st->card[1]].suit != first)
                    k = 1;
            }
            break;
        }
        if (k >= 0)
            ai_Unk4a79d0(st, k);
    }
}
