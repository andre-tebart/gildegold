/* 0x4d2680, the closest "sp_CONQUER" object to the character c, the distance weighted by the owner (+0x200: 5eb990 for
   an own friend, 5eb7b0 for another one that is not a friend, 5ebbe8 otherwise); 0 when it is owned by c itself. Rough draft. */

char *ai_Unk4d2680(char *c)
{
    float v[3];
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    char *m, *o, *e;
    int i;

    ai_Unk6f0d60 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0x40, ai_Unk4d2650, c);
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    d3_PointToWorld(m, m + 0x4c, v);
    for (i = 0; i < ai_Unk6f0d60; i++) {
        o = ai_Unk930380[i];
        dx = v[0] - *(float *)(o + 0x4c);
        dy = v[1] - *(float *)(o + 0x50);
        dz = v[2] - *(float *)(o + 0x54);
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        e = *(char **)(o + 0x200);
        if (e != 0) {
            if (ai_Unk4ce270(c, e) != 0)
                d = (float)(d * ai_Unk5eb990);
            else if (e != 0) {
                if (ai_Unk4ce270(c, e) == 0 && e != c)
                    d = (float)(d * ai_Unk5eb7b0);
                else if (e != 0)
                    d = (float)(d * ai_Unk5ebbe8);
            }
        }
        if (d < best) {
            best = d;
            res = o;
        }
    }
    if (*(char **)(res + 0x200) == c)
        return 0;
    return res;
}
