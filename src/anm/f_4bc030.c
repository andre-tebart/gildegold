/* 0x4bc030, dogs and cats (kinds 0 / 1; name from the string it puts into its actions): mostly stand (dogs also
   bark), else walk to 1..3 points near where they were created
   @deep Q4: 2 raw accesses (CHARACTER anim test at 0x128, d3_DefaultScene+0xb0 hmap); typed fields need ch.h/d3.h in anm.c, blocked by rd.c's char* d3_DefaultScene arithmetic (see notes/units/anm.md) */

void anm_UpdateCat(ANMREC *a)
{
    int px;
    char *h;
    int pz;
    float py;
    int n;
    float pts[8][4];
    float (*p)[4];
    char *act;

    if (*(int *)((char *)a->obj + 0x128) != 0)
        return;
    if (util_Random(100) > 0x1e) {
        if (a->kind == 0) {
            if (util_Random(100) > 0x32)
                ch_QueueAnim(a->obj, "bewegung/stehen", util_Random(3) + 1);
            else
                ch_QueueAnim(a->obj, "bewegung/bellen", util_Random(3) + 1);
        } else
            ch_QueueAnim(a->obj, "bewegung/stehen", util_Random(3) + 1);
        return;
    }
    h = *(char **)((char *)d3_DefaultScene + 0xb0);
    n = util_Random(3) + 1;
    n = rd_PointsNear(a->pos, n, pts);
    if (n > 0) {
        for (p = pts; n != 0; n--, p++) {
            if (d3_GetHeightAt(h, *p, &px, &pz, &py)) {
                act = ch_AddAction(a->obj, 0x2d, px, pz, 0);
                strcpy(act + 0x70, "anm_UpdateCat");
            }
        }
    }
}
