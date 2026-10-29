/* 0x4f8590, the counterpart of 0x4f8460: subtracts the random amount from the cells (+2 of the cell of the object, +0 of its 3 x 3 neighbourhood); no outputs @deep behaviour draft, the same allocation / register-save placement as its sibling ord_AddGridAmount (0x4f8460, escalated in this session with the full reading). Read against tools/disasm.py 0x4f8590: the identical statements with the two subtracts (sub WORD PTR [eax*8+0x91ae62],bx for the cell's +2 sum and the 3x3 loop's +0 sum) and no px / py outputs - no missing or extra statement, constant or offset. Diff: the original saves ebx / esi / edi inside the if and ebp inside the outer loop, spills amount to a stack slot until ebx is free, and keeps y in edi; ours saves edi in the prologue and keeps amount in edi. Tried: unsigned short amount / y (the original's 32-bit slot store and bx add say int), the ORDBLD typed view (kept). Q1 ord_SubGridAmount, Q4 through ORDBLD + ORDCELL, Q7 through gv.h. Needs match-deep's register-allocation / deferred-callee-save toolbox. */

void ord_SubGridAmount(int who, int kind)
{
    int rec[9];
    ORDBLD *b;
    int amount;
    int x;
    int y;
    int xhi;
    int yhi;
    int xlo;
    int ylo;
    int i;
    int j;

    gv_GetTableRecord(kind & 0xff, rec);
    gm_FindById((int *)&b, 0, 0, who);
    amount = util_Random(8);
    y = util_Random(8);
    if (b != 0 && b->object != 0) {
        ord_GetGridCell(b->object + 0x4c, &x, &y);
        ORD_CELLS[y + x * 8].b -= amount;
        xhi = x + 1 > 8 ? 8 : x + 1;
        yhi = y + 1 > 8 ? 8 : y + 1;
        xlo = x - 1 < 0 ? 0 : x - 1;
        for (i = xlo; i <= xhi; i++) {
            ylo = y - 1 < 0 ? 0 : y - 1;
            for (j = ylo; j <= yhi; j++)
                ORD_CELLS[j + i * 8].a -= amount;
        }
    }
}
