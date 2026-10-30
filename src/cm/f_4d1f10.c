/* 0x4d1f10, the selected character that is closest to the character c (distance 0x4ce1b0 times the factor 0x4fc8e0,
   limit 1e6), or 0; the two loops are the same for the kinds 2 / other of c. Rough draft. */

char *ai_Unk4d1f10(char *c)
{
    float best = 1000000.0f;
    float w;
    char *res = 0;
    char **pp;

    if (*(int *)(ai_Unk4cda50(c) + 0x58) == 2) {
        for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            if (*pp != 0 && ai_Unk4ce270(c, *pp) != 0 && (*pp)[8] != 0) {
                w = ai_Unk4ce1b0(c, *(char **)(*(char **)(*pp + 0x184) + 0x34));
                w = unk_Unk4fc8e0(*pp) * w;
                if (w < best) {
                    res = *pp;
                    best = w;
                }
            }
        }
        return res;
    }
    for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        if (*pp != 0 && ai_Unk4ce270(c, *pp) != 0 && (*pp)[8] != 0) {
            w = ai_Unk4ce1b0(c, *(char **)(*(char **)(*pp + 0x184) + 0x34));
            w = unk_Unk4fc8e0(*pp) * w;
            if (w < best) {
                res = *pp;
                best = w;
            }
        }
    }
    return res;
}
