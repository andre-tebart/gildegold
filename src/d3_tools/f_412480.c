/* 0x412480, height range of the map under the quad a, b, c, d */

int d3t_GetQuadRange(D3TMAP *map, float *lo, float *hi, D3VECTOR *a, D3VECTOR *b, D3VECTOR *c, D3VECTOR *d)
{
    float lo1, hi1, lo2, hi2;
    int r1, r2;

    r1 = d3t_GetTriRange(map, &lo1, &hi1, a, b, c);
    r2 = d3t_GetTriRange(map, &lo2, &hi2, c, d, a);
    if (r1 && r2)
        return 1;
    if (!r1 && !r2) {
        *lo = min(lo1, lo2);
        *hi = max(hi1, hi2);
    } else if (!r1) {
        *lo = lo1;
        *hi = hi1;
    } else {
        *lo = lo2;
        *hi = hi2;
    }
    return 0;
}
