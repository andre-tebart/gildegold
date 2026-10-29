/* 0x58ee30, the reverse direction table and entry fill (1117 B, ROUGH): the 68-dword kind table (entry i = the
   label, 2 / 3 / 4) selected by the byte +0x1c of param_2 fills the column titles 0x6f4f0c; the goods of the
   character 0x4ffe90 (param_2 + 10, 1, 5) / 0x4ffcc0 fill the rows 0x6f4f60: an existing id refreshes +0x30
   (0x50aaf0), a new row is created only when the record param_2[0] is not 0x1db / 0x1dc or the object
   0x4ffc40 (param_1 + 0x65, the goods) has a non-zero +4; the second pass 0x6f4f90 refreshes the amounts
   (0x50aaf0 / 0x50ab50, +0x2c = the amount * the count / 4) for the other kinds, and with 0x6f231c & 0x200 and
   0x1467398 the clicked entry 0x5fe710 cycles its state +0x1c (0x4c3ba0 / 0x4c3d90 / 0x4c49e0); the text check
   0x475980 (window +0x18 == 0x41) gives the return 1
   @deep B: 445 code hunks; ROUGH reverse direction table. The 68-dword kind table looked like 0x58e130's (the original has sub esp,0x114 + stack init stores of 2 / 3 / 4), and it is an automatic int tbl[68] (the original fills it on the stack at 0x58ee51.., register stores; a static one has no counterpart in the exe's data and cannot be linked), although that form has more hunks (470 vs 445). No other meaning error found; the row fill / second pass / click cycle are all present, the difference is code shape */

char * gm_FindGoods(int p, unsigned id);

int ai_Unk58ee30(int param_1, short *param_2)
{
    int tbl[68] = {
        2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0,
        4, 0, 0, 0, 3, 2, 0, 0, 4, 3, 2, 0, 4, 3, 3, 0,
        4, 4, 0, 0, 4, 0, 2, 0, 4, 3, 3, 0, 4, 4, 3, 0,
        4, 4, 4, 0, 4, 4, 3, 0, 4, 4, 3, 3, 4, 4, 4, 3,
        4, 4, 4, 4
    };
    int *p;
    unsigned short *g;
    int *q;
    int i;
    int n;
    int st;

    i = 0;
    for (p = (int *)(unsigned)&full_data_6f4f0c; p < (int *)(unsigned)((unsigned)full_data_6f4f0c + 0x50); p += 5, i++)
        *p = tbl[i + ((unsigned char *)param_2)[0x1c] * 4];
    g = (unsigned short *)gm_Query(U_I((char *)param_2 + 10), 1, 5);
    while (g != 0) {
        n = 0;
        for (p = (int *)(unsigned)&full_data_6f4f60; p < (int *)(unsigned)((unsigned)full_data_6f4f60 + 0x380); p += 0xe, n++) {
            if (*p == *g) {
                if (n < 0x10)
                    *(int *)((unsigned)&full_data_6f4f90 + n * 0x38) = gm_ListAmount((int)param_2, g);
                break;
            }
            if (*p == 0) {
                q = (int *)gm_FindGoods(U_I(param_1 + 0x65), *g);
                if (n < 0x10 && ((*param_2 != 0x1db && *param_2 != 0x1dc) || q == 0 || q[1] != 0)) {
                    *(int *)((unsigned)&full_data_6f4f8c + n * 0x38) = 0;
                    *(int *)((unsigned)&full_data_6f4f60 + n * 0x38) = *g;
                    *(int *)((unsigned)&full_data_6f4f90 + n * 0x38) = 0;
                    *(int *)((unsigned)&full_data_6f4f94 + n * 0x38) = 0;
                }
                break;
            }
        }
        g = (unsigned short *)gm_QueryNext();
    }
    for (q = (int *)(unsigned)&full_data_6f4f90; q < (int *)(unsigned)((unsigned)full_data_6f4f90 + 0x380); q += 0xe) {
        g = (unsigned short *)gm_Query(U_I((char *)param_2 + 10), 1, 0, q[-0xc]);
        if (g == 0) {
            q[-0xc] = 0;
            continue;
        }
        if (*param_2 == 0x1db || *param_2 == 0x1dc) {
            n = gm_FindGoods(U_I(param_1 + 0x65), *g);
            if (n != 0 && U_I(n + 4) != 0) {
                q[0] = gm_ListAmount((int)param_2, g);
                q[1] = gm_ListStock((int)param_2, *g);
            }
        } else {
            q[-1] = (gm_ListStock((int)param_2, *g) * U_I((char *)g + 0xe * 2)) >> 2;
            q[0] = gm_ListAmount((int)param_2, g);
            q[1] = gm_ListStock((int)param_2, *g);
        }
    }
    if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0 && U_I(((unsigned)&dm_L + 0x18)) != 0 && d2_CountPointerItems() == 0) {
        for (p = (int *)(unsigned)&full_data_6f4f80; p < (int *)(unsigned)((unsigned)full_data_6f4f80 + 0x380); p += 0xe) {
            if (*p == -1 || *p != U_I((unsigned)&ai_Unk5fe710))
                continue;
            n = gm_Query(U_I((char *)param_2 + 10), 1, 0, p[-8]);
            if (n == 0)
                break;
            st = U_I(n + 0x1c) < 4 ? U_I(n + 0x1c) + 1 : 0;
            cm_BeginFieldBlock(n, U_I(n + 2));
            cm_AddFieldValue(4, 1, (n - U_I((unsigned)&cm_FieldObject)) + 0x1c, &st);
            cm_SendFieldValues();
            U_I(n + 0x1c) = st;
        }
    }
    i = 0;
    for (p = (int *)(unsigned)&full_data_6f4f70; p <= (int *)(unsigned)((unsigned)full_data_6f4f70 + 0x37f); p += 0xe) {
        n = *p;
        if (n != -1 && U_I(U_I((unsigned)&d2_Objs) + 0x18 + n * 0x2ec) == 0x41 && d2_GetObjValue(n) != 0) {
            i = 1;
            break;
        }
    }
    return i;
}
