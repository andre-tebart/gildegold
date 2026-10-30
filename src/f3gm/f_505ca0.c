/* 0x505ca0, fills out[0 .. n-1] with 0 .. n-1 and shuffles it with (n / 2 + max (1, n / 4)) random swaps of different places (n > 1) */

void gm_Shuffled(unsigned char n, int *out)
{
    int i, cnt, swaps;
    unsigned a, b;
    int t;
    unsigned m;

    i = 0;
    if (n != 0) {
        do {
            out[i] = i;
            i++;
        } while (i < (int)n);
    }
    if (1 < n) {
        if ((n & 0xfc) < 5)
            m = 1;
        else
            m = n >> 2;
        cnt = (n >> 1) + m;
        swaps = 0;
        if (cnt != 0) {
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
