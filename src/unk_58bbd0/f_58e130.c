/* 0x58e130, the direction table and the entry fill of the production window (769 B, ROUGH): the 68-dword kind
   table (entry i = the label of the goods kind i) selected by the direction byte +0x1c of param_1 fills the
   column titles 0x6f4f0c (i * 5); the goods of the carrier 0x4ffe90 (param_2 + 0x14, 1, 5) walk through
   0x4ffcc0: an entry of the list 0x6f4f60 is matched by its id (its +0x30 = 0) or a new row is created
   (the id and the three zeros); after the walk the amounts (0x50aaf0 / 0x50ab50 on the character records
   param_1 + 0x14) refresh the rows 0x6f4f90, and the text check 0x475980 of the controls 0x6f4f70 (window +0x18
   == 0x41) gives the return 1
   @deep B: 186 code hunks left after the one meaning fix the original's frame proves: the 68-dword kind table is a local automatic array (the original does sub esp,0x110 and initialises every entry on the stack), not the static const it was; 255 -> 186 hunks. What remains is code shape: the original's init stores interleave in another order (e.g. it writes 0x74, 0x78, 0x88, 0x98 .. with edx/esi where ours writes 0x74 early with ecx), the table walks compare with jl in the original and jb here (pointer compare), and the row search keeps the id in dx and walks a pointer with add 0x38 / inc / cmp / jl against our mov di,WORD PTR + cmp eax,esi. No other missing statement or constant. Q1/Q2/Q3/Q4 also open */

void d2_SetObjValue();

int ai_Unk58e130(int param_1, int param_2)
{
    int tbl[68] = {
        2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0,
        4, 0, 0, 0, 3, 2, 0, 0, 4, 3, 2, 0, 4, 3, 3, 0,
        4, 4, 4, 4, 4, 3, 4, 4, 4, 3, 3, 0, 4, 4, 3, 0,
        4, 4, 4, 0, 4, 4, 3, 0, 4, 4, 3, 3, 4, 4, 4, 3,
        4, 4, 4, 4
    };
    int *p;
    unsigned short *g;
    int *q;
    int i;
    int n;

    i = 0;
    for (p = (int *)(unsigned)&full_data_6f4f0c; p < (int *)(unsigned)((unsigned)full_data_6f4f0c + 0x50); p += 5, i++)
        *p = tbl[i + ((unsigned char *)param_1)[0x1c] * 4];
    g = (unsigned short *)gm_Query(U_I(param_2 + 0x14), 1, 5);
    while (1) {
        if (g == 0) {
            for (p = (int *)(unsigned)&full_data_6f4f90; p < (int *)(unsigned)((unsigned)full_data_6f4f90 + 0x380); p += 0xe) {
                q = (int *)gm_Query(U_I(param_1 + 0x14), 1, 0, p[-0xc]);
                if (q != 0) {
                    p[0] = gm_ListAmount(param_1, q);
                    p[1] = gm_ListStock(param_1, *q);
                }
            }
            i = 0;
            for (p = (int *)(unsigned)&full_data_6f4f70; p <= (int *)(unsigned)((unsigned)full_data_6f4f70 + 0x37f); p += 0xe) {
                n = *p;
                if (n != -1 && U_I(U_I((unsigned)&d2_Objs) + 0x18 + n * 0x2ec) == 0x41 && d2_GetObjValue(n) != 0)
                    i = 1;
            }
            return i;
        }
        n = 0;
        for (p = (int *)(unsigned)&full_data_6f4f60; p < (int *)(unsigned)((unsigned)full_data_6f4f60 + 0x380); p += 0xe, n++) {
            if (*p == *g) {
                if (n < 0x10)
                    *(int *)((unsigned)&full_data_6f4f90 + n * 0x38) = 0;
                break;
            }
            if (*p == 0) {
                if (n < 0x10) {
                    *(int *)((unsigned)&full_data_6f4f60 + n * 0x38) = *g;
                    *(int *)((unsigned)&full_data_6f4f8c + n * 0x38) = 0;
                    *(int *)((unsigned)&full_data_6f4f90 + n * 0x38) = 0;
                    *(int *)((unsigned)&full_data_6f4f94 + n * 0x38) = 0;
                }
                break;
            }
        }
        g = (unsigned short *)gm_QueryNext();
    }
}
