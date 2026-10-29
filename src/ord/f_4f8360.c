/* 0x4f8360, fills the 8 x 8 grid ord_GridCells (cells of 0x18 bytes: +4 the number of objects, +8 the average):
   counts the objects of kind 6 (0x501190 / 0x501030 iteration; +0x61 a pointer, its +0x4c the position through
   0x4f8300 -> x, y) per cell with the value 0x509b10 (o, o[0x5c]) summed up in a local float table, then cell +8
   = sum / count (0 for empty cells). No result.
   @deep behaviour draft, allocation only: the count-clear loop's induction variable. The original keeps the row base in edx and copies it to the cursor (mov edx,0x91ae64 hoisted above the prologue pushes, mov eax,edx in the loop head, mov edx,eax after the inner loop, cmp eax,0x91b464); ours lets VC6 collapse the nested 8x8 clear into one pointer induction variable (mov eax,0x91ae64 in the preheader after the rep stos, cmp eax,0x604, jl). Everything else matches: the object loop, the average loop (ebx counter, esi cursor, cmp 0x91b608), the frame 0x10c and the three callee-saved pushes; the remaining hunks are 4 code (those 3 instructions) + slots + fill. Read against tools/disasm.py 0x4f8360 0x100: same statements, constants and callees. Tried and reverted (each built, obj checked): for(i=0;i<8;i++)for(j=0;j<8;j++) grid[i][j].used=0 flat and 2D-typed (extern ORDCELL grid[8][8], also via a flat (ORDCELL*) cast); ORDCELL *row = grid[i] with an inner j; char *row walk from grid with *(row+j*0x18+4)=0 and from grid+4 with row[j*0x18]; unsigned char *row over &grid[0].used (folds to one IV too); the clear before vs after the memset(f,0,sizeof f); separate i/j counters instead of the address-taken x/y. spell.py: reload/dup-tail/slots leads describe the same allocation (the x/y stack slots 0x10/0x14 are swapped: original x at 0x14, ours at 0x10; declaration order of x/y does not change it). Q4 done via the ORDCELL view (gv.h) and the ORDBLD view for the building record; Q1 ord_CountGridObjects, Q2 clean, Q7 through gv.h. Needs match-deep's induction-variable / allocation toolbox. */

#define ORD_CELLS ((ORDCELL *)ord_GridCells)   /* the grid as flat cells (row y, column x = y * 8 + x) */

void ord_CountGridObjects(void)
{
    float f[64];
    ORDBLD *b;
    int y;
    int x;
    int i;
    int j;
    memset(f, 0, sizeof f);
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            ord_GridCells[i][j].used = 0;
    b = (ORDBLD *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (b->object != 0 && ord_GetGridCell(b->object + 0x4c, &x, &y) != 0) {
            i = y + x * 8;
            f[i] = (float)gm_EquipmentValue((unsigned char *)b, b->level);
            ORD_CELLS[i].used++;
        }
        b = (ORDBLD *)gm_BuildNext();
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (ord_GridCells[i][j].used != 0)
                ord_GridCells[i][j].average = (int)(f[i * 8 + j] / ord_GridCells[i][j].used);
            else
                ord_GridCells[i][j].average = 0;
        }
    }
}
