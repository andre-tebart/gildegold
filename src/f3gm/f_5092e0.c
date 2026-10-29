/* 0x5092e0 */

/* 0x509300, in the building's object words (+0x23) the objects with the class byte (+0x1a3) k that exist in the building's list: the sum of their bytes at +0x1e3; with flag the sum is scaled by the factor of 0x508d70 (0.01) */
int gm_BuildingSkill(unsigned char *b, unsigned k, int flag)
{
    char *p;
    int i, s;
    unsigned short *w;

    i = 0;
    s = 0;
    p = ai_BTypes + (unsigned)*b * 0x24d;
    w = (unsigned short *)(p + 0x23);
    do {
        if ((*w & 0x7fff) != 0 && (unsigned char)p[i + 0x1a3] == k) {
            if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, *w & 0x7fff))
                s += (unsigned char)p[i + 0x1e3];
        }
        i++;
        w++;
    } while (i < 0x40);
    if (flag)
        return (int)(gm_BuildingLevel((char *)b) * 0.01 * s + 0.5);
    return s;
}
