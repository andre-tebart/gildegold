/* 0x501f60, like 0x501e40 for buildings without an owner (+0x27 = -1), taking the positions directly from the objects (+0x4c .. +0x54) */

unsigned char *gm_NearestFreeBuilding(unsigned char *b, unsigned kind)
{
    float best, d, dx, dy, dz;
    unsigned char *r, *p;
    int o, bo;

    best = 1e+08f;
    r = 0;
    p = (unsigned char *)gm_BuildQuery(1, 6);
    while (p != 0) {
        if (gm_IsHomeType(p) == 0 && gm_IsType10(p) == 0 && (o = *(int *)(p + 0x61)) != 0 && p != b && *p == kind && *(short *)(p + 0x27) == -1) {
            bo = *(int *)(b + 0x61);
            dx = *(float *)(o + 0x4c) - *(float *)(bo + 0x4c);
            dy = *(float *)(o + 0x50) - *(float *)(bo + 0x50);
            dz = *(float *)(o + 0x54) - *(float *)(bo + 0x54);
            d = (float)sqrt(dx * dx + dy * dy + dz * dz);
            if (d < best) {
                r = p;
                best = d;
            }
        }
        p = gm_BuildNext();
    }
    return r;
}
