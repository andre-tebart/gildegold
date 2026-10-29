/* 0x4d25a0, the closest object of 0x4d2550 to the character c, or 0 */

char *ai_Unk4d25a0(char *c)
{
    float v[4];
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    char *m, *o;
    int i, n;

    ai_Unk6f0d60 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0x40, ai_Unk4d2550, c);
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    d3_PointToWorld(m, m + 0x4c, v);
    n = ai_Unk6f0d60;
    for (i = 0; i < n; i++) {
        o = ai_Unk930380[i];
        dx = v[0] - *(float *)(o + 0x4c);
        dy = v[1] - *(float *)(o + 0x50);
        dz = v[2] - *(float *)(o + 0x54);
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = o;
        }
    }
    return res;
}
