/* 0x4a76d0, the highest card of a hand (group, then rank): of cards 0..4, or 2..4 if high */

int ai_Unk4a76d0(int *hand, int high)
{
    unsigned int i;
    int best, k, g, r;

    i = high != 0 ? 2 : 0;
    best = hand[i];
    if (best < 0)
        return -1;
    if (hand[i + 1] >= 0) {
        g = ai_Unkf83660[best].suit;
        r = ai_Unkf83660[best].rank;
        for (i++; (int)i < 5; i++) {
            k = hand[i];
            if (k < 0)
                return best;
            if (g < ai_Unkf83660[k].suit || (ai_Unkf83660[k].suit == g && r < ai_Unkf83660[k].rank)) {
                g = ai_Unkf83660[k].suit;
                r = ai_Unkf83660[k].rank;
                best = k;
            }
        }
    }
    return best;
}
