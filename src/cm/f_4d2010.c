/* 0x4d2010, the balance of forces at the ground point (a, b) for the character c: each living enemy within the radius adds
   5eb798 / distance, each other friend subtracts it (the sides are the 16 ids of the fight record +0x80 / +0xc0). */

float ai_Unk4d2010(char *c, int a, int b, float limit)
{
    float acc = 0.0f;
    float v[4];
    float d, dx, dy, dz;
    char *own, *oth, *o, *m;
    char *rec = ai_Unk6f0d04;
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int i;

    if (*(int *)(c + 0x16c) == ai_Unk6f0cec) {
        own = rec + 0xc0;
        oth = rec + 0x80;
    } else {
        own = rec + 0x80;
        oth = rec + 0xc0;
    }
    d3sm_GetVertex(sc, a, b, v);
    for (i = 0; i < 16; i++) {
        if (*(int *)(oth + i * 4) != -1) {
            o = ai_Unk4ce130(*(int *)(oth + i * 4));
            if (o != 0 && (unsigned char)o[8] > 0) {
                m = *(char **)(*(char **)(o + 0x184) + 0x34);
                dx = v[0] - *(float *)(m + 0x4c);
                dy = v[1] - *(float *)(m + 0x50);
                dz = v[2] - *(float *)(m + 0x54);
                d = (float)sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    acc += 70.0f / d;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(own + i * 4) != -1) {
            o = ai_Unk4ce130(*(int *)(own + i * 4));
            if (o != 0 && (unsigned char)o[8] > 0 && o != c) {
                m = *(char **)(*(char **)(o + 0x184) + 0x34);
                dx = v[0] - *(float *)(m + 0x4c);
                dy = v[1] - *(float *)(m + 0x50);
                dz = v[2] - *(float *)(m + 0x54);
                d = (float)sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    acc -= 70.0f / d;
            }
        }
    }
    return acc;
}
