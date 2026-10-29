/* 0x4539b0, out = a + (b - a) * t */

void d3_Lerp3(float *a, float *b, float t, float *out)
{
    out[0] = (b[0] - a[0]) * t + a[0];
    out[1] = (b[1] - a[1]) * t + a[1];
    out[2] = (b[2] - a[2]) * t + a[2];
}
