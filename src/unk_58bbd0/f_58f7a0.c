/* 0x58f7a0, collects the placement candidates of a prefix (369 B): d3_ForEachObject (scene 0x5fb5b0, mask
   0x180) with the filter 0x58f6a0 fills the list param_2 (count 0x6f2688); each collected object is dropped again
   when a character of the build query is within 100.0 of its owner (+0x1f8) or of one of its "bk_" named
   children (+0x1fc list); returns the count
   @deep B: 82 code hunks over 369 B. Read the whole original (0x58f7a0..0x58f910): the one meaning difference is that the list pointer is re-read through 0x6f5308 every iteration (ai_BauplatzList[i]) instead of the parameter list[i]; writing that compiles to 384 B against the original's 369 and unitdiff marks it WORSE, so it was reverted. The rest are register / slot / scheduling hunks (the lea pair ecx/edx, the esi reuse for obj and the bk_ walk): a c2prio / variant round. */

int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */

int ai_FindBauplaetze(char *prefix, int *list)
{
    int i;
    int obj;
    int o;
    int it;
    int c;
    char *s;
    float a[4];
    float b[4];

    U_I((unsigned)&ai_BauplatzCount) = 0;
    U_I((unsigned)&ai_BauplatzList) = (int)list;
    d3_ForEachObject(U_I((unsigned)&ai_Unk5fb5b0), 0, 0x180, ai_BauplatzFilter, prefix);
    for (i = 0; i < U_I((unsigned)&ai_BauplatzCount); i++) {
        obj = list[i];
        if (obj == 0)
            continue;
        o = *(int *)(obj + 0x1f8);
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, a);
            for (it = gm_BuildQuery(1, 6); it != 0; it = gm_BuildNext()) {
                c = *(int *)(it + 0x61);
                if (c != 0) {
                    d3_PointToWorld(c, c + 0x4c, b);
                    if (d3_VecEqual(b, a, 100.0f) != 0) {
                        list[i] = 0;
                        goto next;
                    }
                }
            }
        }
        for (s = *(char **)(obj + 0x1fc); s != 0; s = *(char **)(s + 0x1f0)) {
            if (strncmp(s, "bk_", 3) == 0) {
                d3_PointToWorld(s, s + 0x4c, a);
                for (it = gm_BuildQuery(1, 6); it != 0; it = gm_BuildNext()) {
                    c = *(int *)(it + 0x61);
                    if (c != 0) {
                        d3_PointToWorld(c, c + 0x4c, b);
                        if (d3_VecEqual(b, a, 100.0f) != 0) {
                            list[i] = 0;
                            goto next;
                        }
                    }
                }
            }
        }
    next:
        ;
    }
    return U_I((unsigned)&ai_BauplatzCount);
}
