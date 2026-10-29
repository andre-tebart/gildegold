/* 0x4ef000, out[0] = n (0 .. 4), out[1] = n + 2, out[3 .. 5] and out[6 .. 8]: 500 * 32 * i * t and 700 * 32 * i * t (i = 1 .. 3, t = n * 0.25 + 1) as ints */

void unk_Unk4ef000(int n, int *out)
{
    float t, a;
    int i;
    int *p;

    if (n < 4) {
        if (n < 1)
            n = 0;
    } else
        n = 4;
    out[0] = n;
    out[1] = n + 2;
    t = (float)n * 0.25f + 1.0f;
    p = out + 5;
    for (i = 1; i < 4; i++) {
        a = (float)i * t;
        p[-3] = (int)(500.0f * a * 32.0f);
        p[0] = (int)(a * 700.0f * 32.0f);
        p++;
    }
}
