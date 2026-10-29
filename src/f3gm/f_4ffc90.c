/* 0x4ffc90, the distance of the points (a, b) and (c, d) times 2.90816330909729 (the map scale) */

float gm_MapDistance(int a, int b, int c, int d)
{
    return (float)(sqrt(fabs((double)a - (double)c) * fabs((double)a - (double)c) + fabs((double)b - (double)d) * fabs((double)b - (double)d)) * 2.90816330909729);
}
