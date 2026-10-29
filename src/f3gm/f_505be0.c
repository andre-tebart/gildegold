/* 0x505be0, fills the bytes out[0 .. n-1] with 0 .. n-1 and shuffles them (as 0x505ca0 for bytes) */

void gm_ShuffledBytes(unsigned char n, unsigned char *out)
{
    unsigned char i, cnt, swaps, a, b, t, m;
    unsigned char *p;

    i = 0;
    p = out;
    if (n != 0) {
        do {
            *p = i;
            i++;
            p++;
        } while (i < n);
    }
    if (1 < n) {
        if ((n & 0xfc) < 5)
            m = 1;
        else
            m = n >> 2;
        swaps = 0;
        cnt = (n >> 1) + m;
        if (cnt != 0) {
            do {
                a = (unsigned char)util_Random(n);
                b = (unsigned char)util_Random(n);
                if (a != b) {
                    t = out[a];
                    out[a] = out[b];
                    swaps++;
                    out[b] = t;
                }
            } while (swaps < cnt);
        }
    }
}
