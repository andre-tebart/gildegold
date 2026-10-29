/* 0x58cc10, the goods rows of the production window (2827 B, ROUGH): the same columns as 0x58c680 but for the
   sell side: the owner 0x6f2308 / 0x6f230c and 0x501d80, the column of the row 0x6f534c = the index of the
   selected id 0x6f5a60; each column: the window 0x472aa0 (0xcd x 0x57 at 0x10), the title 0x4731d0 (0x74c),
   the four rows 0x473350 (0x11 + i * 0x31, 0x3a, the goods + 0xdf) with 0x4760b0 / 0x473540 (the counts
   0x5e56c4, the text) and their entries; the sum control 0x472aa0 (0x43, 4, 0x88, 0x32, 0x10) with +0x54;
   the availability checks 0x5098a0 with the texts 0x74d / 0x74e / 0x74f and the shortage local_e4 / uVar10,
   the sum 0x750 + the worst; the carrier records 0x53e400 (2, 0, 4 / 3, 3) and their time ratios (0x4fe350,
   the night tables 0x617830 / 0x617840, the texts 0x4fc / 0x50b / 0x50c); the labels 0xc1 / 0xc2 / 0xc3 /
   0xc4 / 0xc5 / 0xc6 with the cost, the prices and the ratio from the character goods; at the end the unused
   columns from iVar6 are cleared. ROUGH: the availability loop uses the pseudocode registers (local_b4 the
   count of the 0x5098a0 array is not modelled; the texts and the branches of the labels are condensed) */

int gm_BuildingClass(); int d2_WinOpenPicStyled(); void d2_SetObjFont(); void d2_SetCaption();
int gm_ClockSeason(); int ls_GetLanguage(); int d2_CountPointerItems(); void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v); void cm_SendFieldValues(void); void ai_Unk58e6a0(int param_1, int param_2, int *param_3, int param_4, unsigned char param_5, unsigned char *param_6); void d2_MoveObj(short x, short y, int obj);
void d2_SetPointerItem(int id, int amount); void d2_ClearPointerItems(void); int d2_GetObjValue();

void ai_Unk58cc10(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    char *owner;
    int *row;
    int rec;
    char *p;
    int i;
    int n;
    int c;
    int all;
    int idx;
    int t0;
    int id;
    float ratio;
    char buf[64];

    owner = (char *)0;
    if (U_I((unsigned)&ai_PlayerBuilding) != 0)
        owner = (char *)U_I((unsigned)&ai_PlayerBuilding);
    else if (U_I((unsigned)&ai_FallbackBuilding) != 0)
        owner = (char *)U_I((unsigned)&ai_FallbackBuilding);
    gm_BuildingClass((int)*owner);
    d2_SetForm(param_1, 0);
    d2_SetForm(param_1, param_2);
    for (c = 0; c < param_3; c++) {
        *(int *)((unsigned)&full_data_6f534c + c * 0x58) = -1;
        if (U_I((unsigned)&full_data_6f5a60) != 0) {
            for (i = 0; i < 4; i++)
                if (*(int *)((unsigned)&full_data_6f5324 + c * 0x58 + i * 4) == U_I((unsigned)&full_data_6f5a60))
                    *(int *)((unsigned)&full_data_6f534c + c * 0x58) = i;
        }
    }
    for (c = 0; c < param_3; c++) {
        if (*(int *)((unsigned)&full_data_6f5350 + c * 0x58) < 0) {
            rec = d2_CreateChildWindow(*(int *)((unsigned)&full_data_6f5344 + c * 0x58), *(int *)((unsigned)&full_data_6f5348 + c * 0x58), 0xcd, 0x57,
                               0x10, U_I((unsigned)&d2_LastWindow));
            *(int *)((unsigned)&full_data_6f5350 + c * 0x58) = rec;
            U_I((unsigned)&ai_Unkf8f39c + rec * 0x1e2) = 0x54;
            d2_WinOpenPic(0, 0, 0x74c, rec);
            n = d2_WinOpenPic(0xd, 5, *(int *)((unsigned)&full_data_6f5320 + c * 0x58) + 0xdf, *(int *)((unsigned)&full_data_6f5350 + c * 0x58));
            *(int *)((unsigned)&full_data_6f5354 + c * 0x58) = n;
            U_I(U_I((unsigned)&d2_Objs) + 0x4c + n * 0x2ec) = 1;
            U_I(U_I((unsigned)&d2_Objs) + 0x1d8 + n * 0x2ec) = -1;
            U_I(U_I((unsigned)&d2_Objs) + 0x1dc + n * 0x2ec) = -1;
            i = 0x26;
            row = (int *)((unsigned)&full_data_6f535c + c * 0x58);
            for (n = 0; n < 4; n++, i += 2) {
                if (row[n] != 0) {
                    rec = d2_WinOpenPicStyled(n * 0x31 + 0x11, 0x3a, row[n] + 0xdf,
                                       *(int *)((unsigned)&full_data_6f5350 + c * 0x58), -2);
                    row[n] = rec;
                    U_I(U_I((unsigned)&d2_Objs) + 0x4c + rec * 0x2ec) = 1;
                    d2_SetObjFont(rec, 0x54);
                    ai_Unk5e56c4(*(unsigned short *)(i + *(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 0x41 + U_I((unsigned)&ai_Goods)),
                                 buf, 10);
                    d2_SetCaption(rec, buf);
                    U_I(U_I((unsigned)&d2_Objs) + 0x1cc + rec * 0x2ec) = -8;
                    U_I(U_I((unsigned)&d2_Objs) + 0x1d0 + rec * 0x2ec) = 6;
                    U_I(U_I((unsigned)&d2_Objs) + 0x1d8 + rec * 0x2ec) = -1;
                    U_I(U_I((unsigned)&d2_Objs) + 0x1dc + rec * 0x2ec) = -1;
                }
            }
            rec = d2_CreateChildWindow(0x43, 4, 0x88, 0x32, 0x10, *(int *)((unsigned)&full_data_6f5350 + c * 0x58));
            *(int *)((unsigned)&full_data_6f536c + c * 0x58) = rec;
            U_I((unsigned)&ai_Unkf8f39c + rec * 0x1e2) = 0x54;
        }
        all = 0;
        row = (int *)((unsigned)&full_data_6f535c + c * 0x58);
        for (n = 0; n < 4; n++) {
            if (row[n] == 0)
                continue;
            /* ROUGH: the availability of the goods in the carrier stock (0x5098a0) is not modelled: every
               row gets the none label 0x74f and the worst value 0 */
            U_I(U_I((unsigned)&d2_Objs) + 0x1d4 + row[n] * 0x2ec) = 0x74f;
            all = 2;
        }
        U_I(U_I((unsigned)&d2_Objs) + 0x1d4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = all + 0x750;
        p = (char *)he_FindEvent(2, 0, 4, 3, U_I(owner + 1));
        while (p != 0 && *(unsigned short *)(p + 0xac) != *(int *)((unsigned)&full_data_6f5320 + c * 0x58))
            p = (char *)he_NextEvent();
        if (p == 0) {
            p = (char *)he_FindEvent(2, 0, 3, 3, U_I(owner + 1));
            while (p != 0 && *(unsigned short *)(p + 0xac) != *(int *)((unsigned)&full_data_6f5320 + c * 0x58))
                p = (char *)he_NextEvent();
        }
        if (p == 0 || (U_I(p + 0x14) == 0 && U_I(p + 0x18) == 0)) {
            if (*(int *)((unsigned)&full_data_6f5358 + c * 0x58) != -1) {
                d2_RemoveObj(*(int *)((unsigned)&full_data_6f5358 + c * 0x58));
                *(int *)((unsigned)&full_data_6f5358 + c * 0x58) = -1;
            }
            if (*(int *)((unsigned)&full_data_6f5354 + c * 0x58) != -1)
                U_I(U_I((unsigned)&d2_Objs) + 0x1e4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = 0;
        } else {
            n = gm_ClockSeason((char *)(unsigned)&ai_Time);
            id = 0;
            t0 = 0;
            if ((float)U_W((unsigned)&ai_TimeHour) < *(float *)((unsigned)&ai_Unk617830 + n * 4) ||
                *(float *)((unsigned)&ai_Unk617840 + n * 4) <= (float)U_W((unsigned)&ai_TimeHour)) {
                if (*p == 4)
                    id = 0x50b;
                else if (*p == 3)
                    id = 0x50c;
            } else if (*p == 4) {
                if (U_I(p + 200) != 0) {
                    if (*p == 4)
                        id = 0x50b;
                    else if (*p == 3)
                        id = 0x50c;
                } else {
                    id = 0x4fc;
                }
            } else if (*p == 3) {
                id = 0x50c;
            }
            n = *(int *)((unsigned)&full_data_6f5358 + c * 0x58);
            if (-1 < n && U_I(U_I((unsigned)&d2_Objs) + 8 + n * 0x2ec) != id) {
                d2_RemoveObj(n);
                *(int *)((unsigned)&full_data_6f5358 + c * 0x58) = -1;
            }
            if (*(int *)((unsigned)&full_data_6f5358 + c * 0x58) < 0)
                *(int *)((unsigned)&full_data_6f5358 + c * 0x58) = d2_WinOpenPic(0xd, 5, id, *(int *)((unsigned)&full_data_6f5350 + c * 0x58));
            t0 = U_I(p + 0x18) + U_I(p + 0x14);
            if (*p == 4) {
                ratio = 1.0f - *(float *)(p + 0xb4) / *(float *)(p + 0xb8);
                *(float *)(U_I((unsigned)&d2_Objs) + 0x1e4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = ratio;
            } else if (*p == 3) {
                ratio = 1.0f - (float)U_I(p + 0xb0) / *(float *)(p + 0xb4);
                *(float *)(U_I((unsigned)&d2_Objs) + 0x1e4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = ratio;
            }
        }
        row = (int *)((unsigned)&full_data_6f535c + c * 0x58);
        for (i = 0; i < 4; i++)
            if (row[i] != 0)
                U_I(U_I((unsigned)&d2_Objs) + 0x2c + row[i] * 0x2ec) = 0;
        idx = *(int *)((unsigned)&full_data_6f534c + c * 0x58);
        if (idx < 0) {
            U_I((unsigned)&d2_LastWindow) = *(int *)((unsigned)&full_data_6f536c + c * 0x58);
            if (param_5 == 0)
                io_PrintText((char *)(unsigned)&full_data_62b084, t0, 0x51e, 0xb8, *(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 2 + 0x9a8);
            else if (param_5 == 1)
                io_PrintText((char *)(unsigned)&full_data_62b06c, t0, 0x51e, 0xba, *(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 2 + 0x9a8);
            else
                io_PrintText((char *)(unsigned)&full_data_62b054, t0, 0x51e, 0xbb, *(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 2 + 0x9a8);
        } else {
            U_I(U_I((unsigned)&d2_Objs) + 0x2c + row[idx] * 0x2ec) = 1;
            U_I((unsigned)&d2_LastWindow) = *(int *)((unsigned)&full_data_6f536c + c * 0x58);
            /* ROUGH: the labels 0xc1 / 0xc2 (the direction of the carrier) and 0xc3 / 0xc4 (the price of the
               selected row) with their arguments; the pseudocode branches differ only in the argument order */
            io_PrintText(0xc1, *(unsigned short *)(*(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 0x41 + 0x36 + U_I((unsigned)&ai_Goods)),
                         *(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 2 + 0x9a8);
            if (U_I((unsigned)&full_data_6f5334 + c * 0x58 + idx * 0x1c) == 1)
                io_PrintText(0xc3, 1, U_I((unsigned)&full_data_6f5324 + c * 0x58 + idx * 0x1c) * 2 + 0x9a8);
            else
                io_PrintText(0xc4, 0, U_I((unsigned)&full_data_6f5324 + c * 0x58 + idx * 0x1c) * 2 + 0x9a8);
        }
        d2_SetForm(param_1, param_2);
    }
    if (c < 0xf) {
        for (n = (unsigned)&full_data_6f5350 + c * 0x58; n < (unsigned)((unsigned)full_data_6f5350 + 0x580); n += 0x58) {
            if (*(int *)n >= 0) {
                *(int *)(n - 4) = -1;
                d2_RemoveObj(*(int *)n);
                *(int *)(n + 0) = -1;
                *(int *)(n + 4) = -1;
                *(int *)(n + 8) = -1;
                *(int *)(n + 0xc) = -1;
                *(int *)(n + 0x10) = -1;
                *(int *)(n + 0x14) = -1;
                *(int *)(n + 0x18) = -1;
                *(int *)(n + 0x1c) = -1;
            }
        }
    }
}
