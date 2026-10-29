/* 0x4eef50, the sum of c (1 .. 3) terms t, t / 2, t / 4: t = (a + 1) * (b * 0.25 + 25) * 0.002 + 0.1 with a clamped to 0 .. 4 and b to 0 .. 100 */

float unk_Unk4eef50(int a, int b, int c)
{
    int k;
    float sum, t;

    if (a < 4) {
        if (a < 1)
            a = 0;
    } else
        a = 4;
    k = a + 1;
    if (b < 100) {
        if (b < 1)
            a = 0;
        else
            a = b;
    } else
        a = 100;
    if (c < 3) {
        if (c < 2)
            c = 1;
    } else
        c = 3;
    sum = 0.0f;
    if (c > 0) {
        t = (float)k * ((float)a * 0.25f + 25.0f) * 0.002f + 0.1f;
        do {
            sum = sum + t;
            t = t * 0.5f;
            c--;
        } while (c != 0);
    }
    return sum;
}
