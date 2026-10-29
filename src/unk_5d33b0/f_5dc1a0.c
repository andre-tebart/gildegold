/* 0x5dc1a0, the sphere volume record (89 B): radius and size plus the metrics derived from the size
   into out (+0 = radius, +4 = size, +8 = size^2 * 2 pi, +0xc = size^3 * 4/3 pi, +0x10 = that *
   radius). Returns 0 when a negative value comes in, 1 otherwise. */

int ai_SphereVolume(float radius, float size, float *out)
{
    float volume;

    if (radius < 0.0f || size < 0.0f)
        return 0;
    out[0] = radius;
    out[1] = size;
    out[2] = size * size * 6.2831855f;
    volume = size * size * size * 4.1887903f;
    out[3] = volume;
    out[4] = volume * radius;
    return 1;
}
