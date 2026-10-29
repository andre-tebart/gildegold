/* 0x4bc390, n points pts near pos (a free cell within -4..5 cells, else pos's cell); returns n @deep behaviour: remaining code hunks are allocation only - pos in ebx vs the original's edi, frame 0x14 vs 0x10 (x in a frame slot where the original reuses the pos argument slot), and the free-cell fallback stores px/pz into x/z where the original forwards them into the arg pushes; clamps now match (inline h->size-2 < x makes VC6 emit add -2 + cmp/jge; a local m gives sub + jle); Q4 fixed with G15GRID/G15SCENE, readable */

int rd_PointsNear(float *pos, int n, float (*pts)[4])
{
    G15GRID *h;
    int px, pz;
    float py;
    int x, z;
    int i;

    h = ((G15SCENE *)d3_DefaultScene)->hmap;
    d3_GetHeightAt(h, pos, &px, &pz, &py);
    for (i = n; i > 0; i--) {
        x = util_Random(10) - 4 + px;
        z = util_Random(10) - 4 + pz;
        if (h->size - 2 < x)
            x = h->size - 2;
        if (x < 2)
            x = 2;
        if (h->size - 2 < z)
            z = h->size - 2;
        if (z < 2)
            z = 2;
        if (!ch_FindFreeCell((char *)h, pos, pos, x, z, &x, &z, 600, 0)) {
            x = px;
            z = pz;
        }
        d3sm_GetVertex((char *)h, x, z, *pts);
        pts++;
    }
    return n;
}
