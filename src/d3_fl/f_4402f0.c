/* 0x4402f0, linear ramp from v0 at p[a] to v1 at p[b] */

void d3fl_Ramp(unsigned char *p, int a, int b, unsigned char v0, unsigned char v1)
{
    float step = (float)(v1 - v0) / (b - a);
    float f = v0;
    int i;

    for (i = a; i <= b; i++) {
        p[i] = (unsigned char)(f + 0.5f);
        f += step;
    }
}
