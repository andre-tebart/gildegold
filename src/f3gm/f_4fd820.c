/* 0x4fd820, finds a free home among the buildings of the group (1, 5, 1) (kind capacity 0x4f8890 above the occupancy byte +0x65, flag +0x5a bit 0 clear): the nearest to the building of the object o (+0x61) when both have one, else the one with the lowest occupancy ratio; none: a random one of the group (1, 5, 0x16) with the flag clear. ROUGH */

char *unk_Unk4fd820(int unused, char *o)
{
    float best, d, dx, dy, dz;
    char *b, *pick;
    unsigned cap, pickcap, c2;
    int pos[3], pos2[3];
    int list[16];
    int n, k;
    int s[4], s2[4];
    float v1[3], v2[3];

    pick = 0;
    best = 1e+08f;
    ord_GetKindRecord(1, (int *)&cap);
    pickcap = cap;
    b = (char *)gm_BuildQuery(1, 5, 1);
    if (b != 0) {
        do {
            ord_GetKindRecord(*(unsigned char *)b, (int *)s);
            c2 = s[0];
            if ((unsigned short)(unsigned char)b[0x65] < (unsigned short)c2 && (b[0x5a] & 1) == 0) {
                if (o == 0 || *(int *)(o + 0x61) == 0 || *(int *)(b + 0x61) == 0) {
                    if (pick == 0) {
                        pickcap = c2;
                        pick = b;
                    } else {
                        cap = pickcap & 0xffff;
                        if ((float)(unsigned char)b[0x65] / (float)(c2 & 0xffff) < (float)(unsigned char)pick[0x65] / (float)cap)
                            pick = b;
                    }
                } else {
                    d3_PointToWorld(*(int *)(b + 0x61), *(int *)(b + 0x61) + 0x4c, v1);
                    d3_PointToWorld(*(int *)(o + 0x61), *(int *)(o + 0x61) + 0x4c, v2);
                    dx = v1[0] - v2[0];
                    dy = v1[1] - v2[1];
                    dz = v1[2] - v2[2];
                    d = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    if (d < best) {
                        pickcap = c2;
                        pick = b;
                        best = d;
                    }
                }
            }
            b = (char *)gm_BuildNext();
        } while (b != 0);
        if (pick != 0)
            return pick;
    }
    b = (char *)gm_BuildQuery(1, 5, 0x16);
    n = 0;
    memset(list, 0, sizeof(list));
    for (; b != 0 && (b[0x5a] & 1) == 0 && n < 0x10; n++) {
        list[n] = (int)b;
        b = (char *)gm_BuildNext();
    }
    return (char *)list[util_Random(n) & 0xffff];
}
