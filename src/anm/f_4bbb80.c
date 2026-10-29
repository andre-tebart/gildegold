/* 0x4bbb80, kind 2 (once, unk08 -1): picks one of the buildings of the 0x501190(1, 6) list with an object (0x61)
   and collects the positions of the others within 6000 of it (into a local list, which is dropped); unk08 = their
   count * 3
   @deep Q2 + Q4: the gm_BuildQuery/501030/502520/5024f0 callees are f3gm.c's gm_BuildQuery/BuildNext/IsHomeType/IsType10 (AIBLD* return; a void* declaration in anm.h is a new lint cross-TU conflict) and 3 raw *(T*) building accesses at +0x61 (see notes/units/anm.md) */

void anm_UpdateBird(ANMREC *a)
{
    char *b, *f;
    unsigned short n, k;
    int i;
    float p[4];
    char *e;
    float q[4];
    float pts[16][4];
    char *bs[256];

    if (a->nPoints != -1)
        return;
    n = 0;
    for (b = gm_BuildQuery(1, 6); b != 0; b = gm_BuildNext()) {
        if (!gm_IsHomeType(b) && !gm_IsType10(b) && *(int *)(b + 0x61) != 0)
            bs[n++] = b;
    }
    f = bs[util_Random(n)];
    e = *(char **)(f + 0x61);
    d3_PointToWorld(e, e + 0x4c, q);
    k = 0;
    for (i = 0; i < n && k < 0x10; i++) {
        b = bs[i];
        e = *(char **)(b + 0x61);
        d3_PointToWorld(e, e + 0x4c, p);
        if (b != f && d3_VecEqual(q, p, 6000.0f)) {
            pts[k++][0] = p[0];
            pts[k++][1] = p[1];
            pts[k++][2] = p[2];
        }
    }
    a->nPoints = k;
}
