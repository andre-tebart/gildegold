/* 0x4f8920, rebuilds the three float sums (+0xc, +0x10, +0x14) of every cell of the 8 x 8 grid 0x91ae60 from the 0x200 records of 0x2d bytes at 0x91f700 (+0x25 a pointer, +0 an id, -1 = free): the object with the id at +0x21 (0x501190 (1, 1, id)) must have a +0x61 record and a building kind (ai_BTypes[kind * 0x24d]) outside 0xb .. 0xd and not 0x10; its cell x, y (0x4f8300), the table record of the byte +0x1c (0x4f2720): w = (short)(+0x22) / rec[2] * rec[1]; the center cell gets w * 0x5eb7ac at +0xc, each cell of the 3 x 3 neighbourhood w at +0x10 and w * 0x5eb7ac at +0xc @deep far draft (111 code / 16 fpu / 1 fill hunks, 378 of 448 bytes). Read against tools/disasm.py 0x4f8920 0x260: the per-cell float clear (3 stores of a zero register, 8 x 8 nested), the 0x91f700 record walk (p < 0x925100, stride 0x2d, skip when the +0x25 int is 0 or the +0 id is -1), the 0x2d-byte copy, gm_BuildQuery(1, 1, r[0x21]), the guards (o[0] != 0, o->object != 0), the ai_BTypes kind byte (kind * 0x24d, rejected 0xb..0xd and 0x10), ord_GetGridCell(object + 0x4c, &x, &y), gv_GetTableRecord(r[0x1c], rec), w = (short)r[0x22] / rec[2] * rec[1] (the original's fild/fidiv/fimul - my double spelling compiled identically, so the float spelling is kept for the reading), the +0xc += w * 0.5f of the cell, the xhi/yhi/xlo/ylo clamps and the 3 x 3 loop (+0x10 += w, +0xc += w * 0.5f per cell) all match - no missing or extra statement, constant or offset. What is left is structural: the original's nested 8 x 8 clear (the same two-level induction variable that blocks 0x4f8360) and its countdown inner loop of the 3 x 3 block (fld w / fadd [eax+4] / fld w2 / fadd [eax-0x18] with the pointer advanced 0x18), plus the ebx/ebp/esi/edi save placement (the original saves them after the clear, ebp never at all) - allocation / scheduling = match-deep. Tried and measured: the float conversion spelling (byte-identical), the loop bound written gv_Graveyard + 0x5a00 instead of gv_GraveyardEnd (WORSE, 36 bytes - reverted). Q1 ord_RebuildGridSums, Q2 clean (gv_Graveyard / gv_GraveyardEnd named from gv.c's own wording, ORDCELL's floats named halfWeightSum / weightSum / buildingCount). Q4 left as the evidence gap: 5 raw accesses into the 0x2d-byte graveyard record (the int at +0x25, the id at +0, and the copy's +0x1c / +0x21 / +0x22); gv.c reads the same record with raw offsets and names no field, so the +0x1c kind byte / +0x21 id / +0x22 short / +0x25 state have no evidenced names to put in a typed view (a shared-header decision for match-deep). */

void ord_RebuildGridSums(void)
{
    char *p;
    char r[0x2d];
    int rec[9];
    int x;
    int y;
    float w;
    float w2;
    char *o;
    char *b;
    char *q;
    int i;
    int j;
    int xhi;
    int yhi;
    int xlo;
    int ylo;
    unsigned char k;

    for (i = 0; i < 64; i++) {
        ORD_CELLS[i].halfWeightSum = 0;
        ORD_CELLS[i].weightSum = 0;
        ORD_CELLS[i].buildingCount = 0;
    }
    for (p = gv_Graveyard; p < gv_GraveyardEnd; p += 0x2d) {
        if (*(int *)(p + 0x25) == 0 || *(int *)p == -1)
            continue;
        memcpy(r, p, 0x2d);
        o = (char *)gm_BuildQuery(1, 1, *(int *)(r + 0x21));
        if (o == 0 || o[0] == 0)
            continue;
        b = ai_BTypes + (unsigned char)o[0] * 0x24d;
        q = *(char **)(o + 0x61);
        if (q == 0)
            continue;
        k = (unsigned char)b[0];
        if ((k >= 0xb && k <= 0xd) || k == 0x10)
            continue;
        if (ord_GetGridCell(q + 0x4c, &x, &y) == 0)
            continue;
        gv_GetTableRecord((unsigned char)r[0x1c], rec);
        w = (float)*(short *)(r + 0x22) / (float)rec[2] * (float)rec[1];
        w2 = w * 0.5f;
        ORD_CELLS[y + x * 8].halfWeightSum += w2;
        xhi = x + 1 > 8 ? 8 : x + 1;
        yhi = y + 1 > 8 ? 8 : y + 1;
        xlo = x - 1 < 0 ? 0 : x - 1;
        for (i = xlo; i <= xhi; i++) {
            ylo = y - 1 < 0 ? 0 : y - 1;
            for (j = ylo; j <= yhi; j++) {
                ORD_CELLS[j + i * 8].weightSum += w;
                ORD_CELLS[j + i * 8].halfWeightSum += w2;
            }
        }
    }
}
