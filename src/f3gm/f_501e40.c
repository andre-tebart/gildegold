/* 0x501e40, the nearest other building of the type kind to the building b (both need an object at +0x61; not a home (0x502520), not a type-10 (0x5024f0) one): by the squared-root distance of the positions 0x458a20 gives, 0 when none */

unsigned char *gm_NearestBuilding(unsigned char *b, unsigned kind)
{
    float best, d;
    unsigned char *r, *p, *k;
    int o;
    float v1[3], v2[3];

    r = 0;
    best = 1e+08f;
    if (b != 0 && *(int *)(b + 0x61) != 0) {
        p = (unsigned char *)gm_BuildQuery(1, 6);
        while (p != 0) {
            k = (unsigned char *)ai_BTypes + *p * 0x24d;
            if (gm_IsHomeType(p) == 0 && gm_IsType10(p) == 0 && (o = *(int *)(p + 0x61)) != 0 && p != b && *k == kind) {
                d3_PointToWorld(o, o + 0x4c, v1);
                d3_PointToWorld(*(int *)(b + 0x61), *(int *)(b + 0x61) + 0x4c, v2);
                d = (float)sqrt((v1[0] - v2[0]) * (v1[0] - v2[0]) + (v1[1] - v2[1]) * (v1[1] - v2[1]) + (v1[2] - v2[2]) * (v1[2] - v2[2]));
                if (d < best) {
                    r = p;
                    best = d;
                }
            }
            p = gm_BuildNext();
        }
        return r;
    }
    return 0;
}
