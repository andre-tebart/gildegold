/* 0x58c680, the column builder of the production window (1421 B, ROUGH): the columns 0x6f5320 / 0x6f5350 /
   0x6f5354 / 0x6f5358 / 0x6f536c (0x58 bytes each): for each of the param_3 columns the carrier goods 0x6f5320
   (the record of the character 0x91aaf0 + id * 0x41 words), the window 0x472aa0 (0xcd x 0x57 at 0x10) and the
   title 0x731d0 (0x749 / 0x74a), the four goods rows 0x473350 (0x11 + i * 0x31, 0x3a, the goods + 0xdf) with the
   window entries (+0x4c = 1, +0x1d8 / +0x1dc = -1), the sum control 0x472aa0 (0x43, 4, 0x88, 0x32) and its
   entry +0x54, the availability 0x5098a0 (the carrier's goods, the list local_b8 / the amounts): a matching
   row gets the text 0x74d (enough) / 0x74e (short) / 0x74f (none) and the sum 0x750 + the worst; the carrier
   record 0x53e400 (2, 0, 9 / 8, 3) gives the time ratio (0x4fe280) into the control +0x1e4; the screen
   0x6f536c; the window control param_2 is re-set each column; at the end the unused columns from iVar6 are
   cleared (0x6f5350 + i * 0x58 < 0x6f58d0). ROUGH: the goods / row indices are kept from the pseudocode; the
   out array local_b8 (0x5098a0) and its count local_b4 are not modelled */

int gm_BuildingGoodsList(char *b, unsigned *out, int kind); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win); struct AIPER; struct D3VECTOR; int d2_WinOpenPic(short x, short y, int pic, int win); int d2_WinOpenPicStyled();
void d2_RemoveObj(); int gm_MinutesBetween(); char * he_FindEvent(int n, ...); char * he_NextEvent(void);
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...);

void ai_Unk58c680(int param_1, int param_2, int param_3, int param_4)
{
    int *col;
    int *row;
    int rec;
    int i;
    int n;
    int c;
    int sum;
    int all;
    int t0;
    int t1;
    unsigned short have[16];
    int amount[20];
    int tgt;

    tgt = 0;
    if (U_I((unsigned)&ai_PlayerBuilding) != 0)
        tgt = U_I((unsigned)&ai_PlayerBuilding);
    else if (U_I((unsigned)&ai_FallbackBuilding) != 0)
        tgt = U_I((unsigned)&ai_FallbackBuilding);
    d2_SetForm(param_1, 0);
    d2_SetForm(param_1, param_2);
    gm_BuildingGoodsList(tgt, (char *)have, 1);
    for (c = 0; c < param_3; c++) {
        col = (int *)((unsigned)&full_data_6f5320 + c * 0x58);
        if (*(int *)((unsigned)&full_data_6f5350 + c * 0x58) < 0) {
            rec = d2_CreateChildWindow(*(int *)((unsigned)&full_data_6f5344 + c * 0x58), *(int *)((unsigned)&full_data_6f5348 + c * 0x58), 0xcd, 0x57,
                               0x10, U_I((unsigned)&d2_LastWindow));
            *(int *)((unsigned)&full_data_6f5350 + c * 0x58) = rec;
            U_I((unsigned)&ai_Unkf8f39c + rec * 0x1e2) = 0x54;
            d2_WinOpenPic(0, 0, 0x749, rec);
            if (c == 0) {
                U_I((unsigned)&full_data_6f5354) = d2_WinOpenPic(0xd, 5, 0x74a, *(int *)((unsigned)&full_data_6f5350));
            } else {
                U_I((unsigned)&full_data_6f5354 + c * 0x58) =
                    d2_WinOpenPic(0xd, 5, *(int *)((unsigned)&full_data_6f5320 + c * 0x58) + 0xdf, *(int *)((unsigned)&full_data_6f5350 + c * 0x58));
            }
            n = *(int *)((unsigned)&full_data_6f5354 + c * 0x58);
            U_I(U_I((unsigned)&d2_Objs) + 0x4c + n * 0x2ec) = 1;
            U_I(U_I((unsigned)&d2_Objs) + 0x1d8 + n * 0x2ec) = -1;
            U_I(U_I((unsigned)&d2_Objs) + 0x1dc + n * 0x2ec) = -1;
            row = (int *)((unsigned)&full_data_6f535c + c * 0x58);
            for (i = 0; i < 4; i++) {
                if (row[-0xe + i] != 0) {
                    rec = d2_WinOpenPicStyled(i * 0x31 + 0x11, 0x3a, row[-0xe + i] + 0xdf,
                                       *(int *)((unsigned)&full_data_6f5350 + c * 0x58), -2);
                    row[i] = rec;
                    U_I(U_I((unsigned)&d2_Objs) + 0x4c + rec * 0x2ec) = 1;
                    U_I(U_I((unsigned)&d2_Objs) + 0x1d8 + rec * 0x2ec) = -1;
                    U_I(U_I((unsigned)&d2_Objs) + 0x1dc + rec * 0x2ec) = -1;
                }
            }
            rec = d2_CreateChildWindow(0x43, 4, 0x88, 0x32, 0x10, *(int *)((unsigned)&full_data_6f5350 + c * 0x58));
            U_I((unsigned)&full_data_6f536c + c * 0x58) = rec;
            U_I((unsigned)&ai_Unkf8f39c + rec * 0x1e2) = 0x54;
        }
        all = 0;
        row = (int *)((unsigned)&full_data_6f535c + c * 0x58);
        for (i = 0; i < 4; i++) {
            if (row[i] == 0)
                continue;
            rec = 0; /* the goods id of the row */
            sum = 0; /* 0x74d .. 0x74f */
            U_I(U_I((unsigned)&d2_Objs) + 0x1d4 + row[i] * 0x2ec) = sum;
            if (all < 2)
                all = 1;
        }
        U_I(U_I((unsigned)&d2_Objs) + 0x1d4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = all + 0x750;
        n = 0;
        if (c == 0)
            rec = he_FindEvent(2, 0, 9, 3, U_I(tgt + 1));
        else
            rec = he_FindEvent(2, 0, 8, 3, U_I(tgt + 1));
        if (rec != 0 && (U_I(rec + 0x14) != 0 || U_I(rec + 0x18) != 0)) {
            if (*(int *)((unsigned)&full_data_6f5358 + c * 0x58) < 0)
                U_I((unsigned)&full_data_6f5358 + c * 0x58) = d2_WinOpenPic(0xd, 5, 0x4fd, *(int *)((unsigned)&full_data_6f5350 + c * 0x58));
            n = U_I(rec + 0x18) + U_I(rec + 0x14);
            t0 = gm_MinutesBetween(rec + 0x44, rec + 0x52);
            t1 = gm_MinutesBetween(rec + 0x44, (unsigned)&ai_Time);
            *(float *)(U_I((unsigned)&d2_Objs) + 0x1e4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = (float)t1 / (float)t0;
        } else {
            if (*(int *)((unsigned)&full_data_6f5358 + c * 0x58) != -1) {
                d2_RemoveObj(*(int *)((unsigned)&full_data_6f5358 + c * 0x58));
                U_I((unsigned)&full_data_6f5358 + c * 0x58) = -1;
            }
            if (*(int *)((unsigned)&full_data_6f5354 + c * 0x58) != -1)
                *(float *)(U_I((unsigned)&d2_Objs) + 0x1e4 + *(int *)((unsigned)&full_data_6f5354 + c * 0x58) * 0x2ec) = 0.0f;
        }
        if (c == 0) {
            U_I((unsigned)&d2_LastWindow) = U_I((unsigned)&full_data_6f536c);
            io_PrintText(200, n, 0x51e);
        } else {
            U_I((unsigned)&d2_LastWindow) = *(int *)((unsigned)&full_data_6f536c + c * 0x58);
            io_PrintText(0xc9, n, 0x51e, *(int *)((unsigned)&full_data_6f5320 + c * 0x58) * 2 + 0x9a8);
        }
        d2_SetForm(param_1, param_2);
    }
    if (c >= 0xf)
        return;
    for (n = (unsigned)&full_data_6f5350 + c * 0x58; n < (unsigned)((unsigned)full_data_6f5350 + 0x580); n += 0x58) {
        if (*(int *)n >= 0) {
            *(int *)(n - 4) = -1;
            d2_RemoveObj(*(int *)n);
            *(int *)n = -1;
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
