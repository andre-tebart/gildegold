/* 0x5241d0, counts the members of the group record g by their home building (+0x16c): +0x14 those whose title byte (+0x165) is the building's byte +0x22f (or, for kind 0x1e, accepted by 0x4eaad0), +0x18 those with +0x230 */

void sv_CountGroupMembers(char *g)
{
    int i;
    int *p;
    char *c;
    char *b;

    *(int *)(g + 0x14) = 0;
    *(int *)(g + 0x18) = 0;
    p = (int *)(g + 0x8c);
    i = 8;
    do {
        if (*p != -1) {
            c = gm_FindChar(*p);
            if (c != 0 && *(unsigned char **)(c + 0x16c) != 0) {
                b = ai_BTypes + **(unsigned char **)(c + 0x16c) * 0x24d;
                if (c[0x165] == b[0x22f]) {
                    (*(int *)(g + 0x14))++;
                } else if (c[0x165] == b[0x230]) {
                    (*(int *)(g + 0x18))++;
                } else if (b[0] == 0x1e) {
                    if (ahm_HasRecord(c) != 0)
                        (*(int *)(g + 0x14))++;
                }
            }
        }
        p++;
        i--;
    } while (i != 0);
}
