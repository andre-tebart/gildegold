/* 0x454690, matrix rows to angles */

void d3_MatToAngles(D3VECTOR4 *m, D3VECTOR4 *angles)
{
    float *f = (float *)m;
    double l = sqrt((double)f[4] * f[4] + (double)f[0] * f[0]);

    if (l > 1.9073486328125e-06) {
        angles->x = (float)atan2(f[9], f[10]);
        angles->y = (float)atan2(-f[8], l);
        angles->z = (float)atan2(f[4], f[0]);
    } else {
        angles->x = (float)atan2(-f[6], f[5]);
        angles->y = (float)atan2(-f[8], l);
        angles->z = 0.0f;
    }
}
