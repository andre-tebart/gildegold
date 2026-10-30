/* 0x505d30, shuffles the n ints at out in place (n / 2 + max (1, n / 4) swaps of different places) */

void gm_Shuffle(unsigned n, int *out)
{
    unsigned m, a, b;
    int cnt, swaps, t;

    if (n >= 2) {
        m = n >> 2;
        if (m <= 1)
            m = 1;
        swaps = 0;
        cnt = (n >> 1) + m;
        if (0 < cnt) {
            do {
                a = util_RandomU(n);
                b = util_RandomU(n) & 0xffff;
                if (a != b) {
                    t = out[a];
                    out[a] = out[b];
                    out[b] = t;
                    swaps++;
                }
            } while (swaps < cnt);
        }
    }
}
