/* 0x58d720, the goods list of the production window (1491 B, ROUGH): the direction bytes +0x1c / +0x1d of the
   building record param_2 select a 54-dword kind table (local_d8) for the two lists 0x6f4f0c / 0x6f52ec; the
   goods of the character 0x4ffe90 (+0x14, 1, 5) / 0x4ffcc0 fill the 0x10-entry list 0x6f4f60 (the kind byte of
   the goods record 0x91aaf0 + id * 0x41: 0x17 / '%' for the one side) and the 8-entry list 0x6f58a0 (the other
   kinds), each entry +0x2c = 0x50ab50 (the amount) * the count (puVar4 + 0xe) / 4, +0x30 = 0x50aaf0, +0x34 =
   0x50ab50; the ids without a record are cleared; the text check 0x475980 of the control +0x18 == 0x41; the
   click 0x5fe710 on the entries 0x6f4f80 / 0x6f58c0 cycles its state +0x1c 0 .. 4 through 0x4c3ba0 / 0x4c3d90
   (4, 1, the field) / 0x4c49e0. Returns 1 when any entry has a text. ROUGH: the kind table local_d8 is
   initialised from the pseudocode values, the entry helpers (0x50ab50 / 0x50aaf0 / 0x50ab50) are kept */

int ai_Unk58d720(unsigned char *param_1, int param_2)
{
    int tbl[54];
    int *p;
    unsigned short *g;
    int i;
    int n;
    int k;
    int st;
    unsigned short id;
    unsigned char kind;

    tbl[0] = 0; tbl[1] = 2; tbl[2] = 0; tbl[3] = 2; tbl[4] = 0; tbl[5] = 2; tbl[6] = 0; tbl[7] = 3;
    tbl[8] = 0; tbl[9] = 4; tbl[10] = 2; tbl[11] = 3; tbl[12] = 2; tbl[13] = 4; tbl[14] = 3;
    tbl[15] = 4; tbl[16] = 4; tbl[17] = 4; tbl[0x12] = 2; tbl[0x13] = 0; tbl[0x14] = 0; tbl[0x15] = 0;
    tbl[0x16] = 2; tbl[0x17] = 0; tbl[0x18] = 0; tbl[0x19] = 0; tbl[0x1a] = 2; tbl[0x1b] = 0;
    tbl[0x1c] = 0; tbl[0x1d] = 0; tbl[0x1e] = 3; tbl[0x1f] = 0; tbl[0x20] = 0; tbl[0x21] = 0;
    tbl[0x22] = 4; tbl[0x23] = 0; tbl[0x24] = 0; tbl[0x25] = 0; tbl[0x26] = 3; tbl[0x27] = 2;
    tbl[0x28] = 0; tbl[0x29] = 0; tbl[0x2a] = 4; tbl[0x2b] = 2; tbl[0x2c] = 0; tbl[0x2d] = 0;
    tbl[0x2e] = 4; tbl[0x2f] = 3; tbl[0x30] = 0; tbl[0x31] = 0; tbl[0x32] = 4; tbl[0x33] = 4;
    tbl[0x34] = 0; tbl[0x35] = 0;
    k = gm_BuildingClass(*param_1);
    for (i = 0, p = (int *)(unsigned)&full_data_6f4f0c; p < (int *)(unsigned)((unsigned)ai_FallbackBuildingB + 0x2c50); p += 5, i++)
        *p = tbl[i + ((unsigned char *)param_2)[0x1c] * 4 + 0x12];
    for (i = 0, p = (int *)(unsigned)&full_data_6f52ec; p < (int *)(unsigned)((unsigned)ai_FallbackBuildingB + 0x3008); p += 5, i++)
        *p = tbl[i + ((unsigned char *)param_2)[0x1d] * 2];
    g = (unsigned short *)gm_Query(U_I(param_1 + 0x14), 1, 5);
    while (g != 0) {
        id = *g;
        kind = *(unsigned char *)(U_I((unsigned)&ai_Goods) + id * 0x41);
        if (k == 2 ? kind == 0x17 : (kind == 0x17 || kind == '%')) {
            n = -1;
            for (i = 0, p = (int *)(unsigned)&full_data_6f4f60; p < (int *)(unsigned)((unsigned)ai_FallbackBuildingB + 0x2fd4); p += 0xe, i++)
                if (*p == id) { n = i; break; }
            if (n < 0) {
                for (i = 0, p = (int *)(unsigned)&full_data_6f4f60; p < (int *)(unsigned)((unsigned)ai_FallbackBuildingB + 0x2fd4); p += 0xe, i++)
                    if (*p == 0) { n = i; break; }
                if (n >= 0)
                    *(int *)((unsigned)&full_data_6f4f60 + n * 0x38) = id;
            }
            if (n >= 0 && n < 0x10) {
                *(int *)((unsigned)&full_data_6f4f8c + n * 0x38) =
                    (gm_ListStock(param_2, id) * *(int *)(g + 0xe)) >> 2;
                *(int *)((unsigned)&full_data_6f4f90 + n * 0x38) = gm_ListAmount(param_2, g);
                *(int *)((unsigned)&full_data_6f4f94 + n * 0x38) = gm_ListStock(param_2, *g);
            }
        } else {
            n = -1;
            for (i = 0, p = (int *)(unsigned)&full_data_6f58a0; p < (int *)(unsigned)((unsigned)ai_FallbackBuildingB + 0x3754); p += 0xe, i++)
                if (*p == id) { n = i; break; }
            if (n < 0) {
                for (i = 0, p = (int *)(unsigned)&full_data_6f58a0; p < (int *)(unsigned)((unsigned)ai_FallbackBuildingB + 0x3754); p += 0xe, i++)
                    if (*p == 0) { n = i; break; }
                if (n >= 0)
                    *(int *)((unsigned)&full_data_6f58a0 + n * 0x38) = id;
            }
            if (n >= 0 && n < 8) {
                *(int *)((unsigned)&full_data_6f58cc + n * 0x38) =
                    (gm_ListStock(param_2, id) * *(int *)(g + 0xe)) >> 2;
                *(int *)((unsigned)&full_data_6f58d0 + n * 0x38) = gm_ListAmount(param_2, g);
                *(int *)((unsigned)&full_data_6f58d4 + n * 0x38) = gm_ListStock(param_2, *g);
            }
        }
        g = (unsigned short *)gm_QueryNext();
    }
    for (p = (int *)(unsigned)&full_data_6f4f60; p < (int *)(unsigned)((unsigned)full_data_6f4f60 + 0x380); p += 0xe)
        if (gm_Query(U_I(param_1 + 0x14), 1, 0, *p) == 0)
            *p = 0;
    for (p = (int *)(unsigned)&full_data_6f58a0; p < (int *)(unsigned)((unsigned)full_data_6f58a0 + 0x1c0); p += 0xe)
        if (gm_Query(U_I(param_1 + 0x14), 1, 0, *p) == 0)
            *p = 0;
    i = 0;
    for (p = (int *)(unsigned)&full_data_6f4f70; p <= (int *)(unsigned)((unsigned)full_data_6f4f70 + 0x37f); p += 0xe) {
        n = *p;
        if (n != -1 && U_I(U_I((unsigned)&d2_Objs) + 0x18 + n * 0x2ec) == 0x41 && d2_GetObjValue(n) != 0) {
            i = 1;
            break;
        }
    }
    if (i == 0) {
        for (p = (int *)(unsigned)&full_data_6f58b0; p <= (int *)(unsigned)((unsigned)full_data_6f58b0 + 0x1bf); p += 0xe) {
            n = *p;
            if (n != -1 && U_I(U_I((unsigned)&d2_Objs) + 0x18 + n * 0x2ec) == 0x41 && d2_GetObjValue(n) != 0) {
                i = 1;
                break;
            }
        }
    }
    if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0 && U_I(((unsigned)&dm_L + 0x18)) != 0 && d2_CountPointerItems() == 0) {
        for (p = (int *)(unsigned)&full_data_6f4f80; p < (int *)(unsigned)((unsigned)full_data_6f4f80 + 0x380); p += 0xe) {
            if (*p == -1 || *p != U_I((unsigned)&ai_Unk5fe710))
                continue;
            n = gm_Query(U_I(param_1 + 0x14), 1, 0, p[-8]);
            if (n == 0)
                break;
            st = U_I(n + 0x1c) < 4 ? U_I(n + 0x1c) + 1 : 0;
            cm_BeginFieldBlock(n, U_I(n + 2));
            cm_AddFieldValue(4, 1, (n - U_I((unsigned)&cm_FieldObject)) + 0x1c, &st);
            cm_SendFieldValues();
            U_I(n + 0x1c) = st;
        }
        for (p = (int *)(unsigned)&full_data_6f58c0; p < (int *)(unsigned)((unsigned)full_data_6f58c0 + 0x1c0); p += 0xe) {
            if (*p == -1 || *p != U_I((unsigned)&ai_Unk5fe710))
                continue;
            n = gm_Query(U_I(param_1 + 0x14), 1, 0, p[-8]);
            if (n == 0)
                return i;
            st = U_I(n + 0x1c) < 4 ? U_I(n + 0x1c) + 1 : 0;
            cm_BeginFieldBlock(n, U_I(n + 2));
            cm_AddFieldValue(4, 1, (n - U_I((unsigned)&cm_FieldObject)) + 0x1c, &st);
            cm_SendFieldValues();
            U_I(n + 0x1c) = st;
        }
    }
    return i;
}
