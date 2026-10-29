/* 0x4f8460, adds a random amount (0x42ec00 (8)) to the sums (+2, a short) of the cells of the 8 x 8 grid 0x91ae60 around the position of an object (0x4fea50 gives the object of who; 0x4f8300 the cell x, y of its +0x61 record's +0x4c): the 3 x 3 neighbourhood, clamped to the grid; the record kind of 0x4f2720 is read first (unused); the cell x, y go to *px / *py @deep behaviour draft, allocation / register-save placement only (109 code hunks, every statement equal). Read against tools/disasm.py 0x4f8460 0x130 and asmdiff: gv_GetTableRecord(kind & 0xff, rec), gm_FindById(&b, 0, 0, who), amount = util_Random(8), y = util_Random(8), the o / o->object guard, ord_GetGridCell(b->object + 0x4c, &x, &y), the +2 sum add (add WORD PTR [eax*8+0x91ae62],bx), the xhi / yhi clamps (mov [esp+0x3c],8 + cmp/jg), the xlo / ylo setl-dec-and clamps and the 3x3 add loop with the index edx + esi*8 all match. Diff: the original does NOT save ebx / esi / edi in the prologue - it saves them inside the if (0x4f84c6) and ebp inside the outer loop (0x4f8532) - and spills amount to [esp+0x20] until ebx is free (the add then uses bx); ours pushes edi in the prologue and keeps amount in edi, so the x / y slots and the register roles swap (ours amount=edi, y=ebx; original amount=ebx, y=edi). Tried: int amount (kept), unsigned short amount / y (the original's add uses bx and a 32-bit slot store, so they are ints), the ORDBLD typed view (kept, byte-neutral), gm_FindById prototyped in gv.h (Q7). Q1 ord_AddGridAmount, Q4 through ORDBLD + ORDCELL, Q7 through gv.h. Needs match-deep's register-allocation / deferred-callee-save toolbox. */

void ord_AddGridAmount(int who, int kind, int *px, int *py)
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
        ORD_CELLS[y + x * 8].b += amount;
        xhi = x + 1 > 8 ? 8 : x + 1;
        yhi = y + 1 > 8 ? 8 : y + 1;
        xlo = x - 1 < 0 ? 0 : x - 1;
        for (i = xlo; i <= xhi; i++) {
            ylo = y - 1 < 0 ? 0 : y - 1;
            for (j = ylo; j <= yhi; j++)
                ORD_CELLS[j + i * 8].a += amount;
        }
        *px = x;
        *py = y;
    }
}
