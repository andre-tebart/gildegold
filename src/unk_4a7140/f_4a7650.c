/* 0x4a7650, the first group (of 4 cards) other than skip of which seat 2 holds two or more cards (only shown ones
   if shown): its index into out, the count as the result, 0 if none */

int ai_Unk4a7650(int *hand, int shown, int skip, int *out)
{
    int cnt[6];
    int s, g, i, n, best;
    G10CARD *c;
    int *q;

    for (s = 1; s < 3; s++) {
        best = 0;
        q = cnt;
        c = ai_Unkf83660;
        do {
            n = 0;
            for (i = 4; i != 0; i--) {
                if (c->owner == s && (shown == 0 || c->shown != 0))
                    n++;
                c++;
            }
            if (best < n)
                best = n;
            *q++ = n;
        } while (c < &ai_Unkf83660[24]);
    }
    for (g = 0; g == skip || cnt[g] < 2;) {
        if (++g > 5)
            return 0;
    }
    *out = g;
    return cnt[g];
}
