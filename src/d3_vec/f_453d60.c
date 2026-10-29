/* 0x453d60, ra = a mod 2pi, rb = b mod 2pi, then ra shifted by +-2pi to be the nearest to rb */

void d3_UnwrapAngles(float *a, float *b, float *ra, float *rb)
{
    int i;
    float x;
    float d;

    for (i = 0; i < 3; i++) {
        ra[i] = (float)fmod(a[i], 6.2831855f);
        x = (float)fmod(b[i], 6.2831855f);
        rb[i] = x;
        d = (float)fabs(x - ra[i]);
        if (d > fabs(x - (ra[i] + 6.2831855f))) {
            ra[i] = ra[i] + 6.2831855f;
        } else if (d > fabs(x - (ra[i] - 6.2831855f))) {
            ra[i] = ra[i] - 6.2831855f;
        }
    }
}
