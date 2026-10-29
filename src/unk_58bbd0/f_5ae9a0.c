/* 0x5ae9a0, the transport from the warehouse (1648 B, ROUGH): the kind record 0x7e3f30 + kind * 0x24d; with no
   building (param_5) the player's one 0x4ffe90 (0, 2, 7, 1, param_1), else the failure text 0x62cc94
   "ob_TransportFromLager(): ...". 0x4c3b30 opens the message, then both carrier tables (0x6f58a0 .. 0x6f5a60
   and 0x6f4f60 .. 0x6f52e0, 0x38-byte rows: the good +0, the control id +0x10, the available +0x30) are walked:
   the amount 0x475980 (0x50acf0 / 0x50ad70 against the building's stock), the price 0x5089c0 (1.1 for a "\n"
   kind, else the float +0x49 of the record; the +0x65 amount for the modes 2 / 4), the character check
   0x521040 of 0x770b80 + 0x6f25b8 * 0x21c, the limit 0x5085f0 and the confirmation 0x576340 (0x25 / 0x2a for
   the +1 flag, 0x27 / 0x29 for the +8 flag) with 0x576040; the order 0x4c4880, the text 0x5176d0 (0xec) /
   0x52e330 / 0x426fa0 ("RollenKurz_s_01" 0x60c28c, 0x3f, 0x7f) and the slider 0x475cb0. 0x4c3b40 then
   0x4c3b50 are waited on (0x52fe00) and 1 returns */

int ai_Unk5ae9a0(int param_1, int param_2, unsigned char *param_3, int param_4, int param_5,
                 float param_6, int param_7, unsigned int param_8)
{
    char text[256];
    float v;
    int rec;
    char *kind;
    int n;
    int m;
    int r;
    int *row;

    rec = 0;
    kind = 0;
    v = 0.0f;
    if (param_3 != 0)
        kind = (char *)(U_I((unsigned)&ai_BTypes) + *param_3 * 0x24d);
    if (param_5 == 0) {
        rec = gm_Query(0, 2, 7, 1, param_1);
        if (rec == 0) {
            err_Message((char *)(unsigned)&full_data_62cc94);
            return 0;
        }
    }
    cm_BeginSendGroup();
    for (row = (int *)(unsigned)&full_data_6f58a0; (int)row < (unsigned)((unsigned)full_data_6f58a0 + 0x1c0); row += 0xe) {
        if (row[4] == -1 || d2_GetObjValue(row[4]) == 0)
            continue;
        n = d2_GetObjValue(row[4]);
        if (param_5 == 0) {
            if (rec != 0)
                n = gm_Unk50acf0(rec, row[0], n);
        } else {
            n = gm_Unk50ad70(param_5, row[0], n);
        }
        if (n <= 0)
            continue;
        if (param_4 == 0) {
            v = 0.0f;
        } else {
            if (*kind == 0xa)
                v = gm_GoodsPrice(row[0], param_7) * 1.1;
            else
                v = gm_GoodsPrice(row[0], param_7) * *(float *)(param_3 + 0x49);
            if (param_8 == 2 || param_8 == 4)
                v = gm_GoodsPrice(row[0], param_3[0x65]);
        }
        if (sv_CheckGoodAmount((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c), (int)param_7) == 0)
            continue;
        if (row[0xc] < 0)
            continue;
        m = gm_PlayerFactor(1, param_7);
        /* ROUGH: the __ftol of the price against the limit m */
        r = (int)v;
        if (r < m)
            param_6 = 1.0f;
        if ((param_8 & 8) == 0) {
            if ((param_8 & 1) == 0) {
                cm_SendExSellObject(param_1, *(int *)(param_2 + 2), row[0], n, param_7, (int)v);
                if (v != 0.0f && param_6 != 1.0f) {
                    txt_Format(text, 0xec, (int)v);
                    sv_SetStatusText(text);
                    sb_PlaySample(0, (char *)(unsigned)"RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
                }
                m = d2_GetObjValue(row[4], 0);
                d2_SetObjValue(row[4], 0, row[0xc], m - n);
            } else if (param_5 == 0) {
                if (ai_Unk576340(0x25, param_3, 0, row[0]) != 0) {
                    ai_Unk576040(0x25, param_3, 0, row[0]);
                    goto order;
                }
            } else if (ai_Unk576340(0x2a, param_3, 0, row[0]) != 0) {
                ai_Unk576040(0x2a, param_3, 0, row[0]);
                goto order;
            }
        } else if (param_5 == 0) {
            if (ai_Unk576340(0x27, param_3, 0, row[0]) != 0) {
                ai_Unk576040(0x27, param_3, 0, row[0]);
                goto order;
            }
        } else if (ai_Unk576340(0x29, param_3, 0, row[0]) != 0) {
            ai_Unk576040(0x29, param_3, 0, row[0]);
order:
            cm_SendExSellObject(param_1, *(int *)(param_2 + 2), row[0], n, param_7, (int)v);
            if (v != 0.0f && param_6 != 1.0f) {
                txt_Format(text, 0xec, (int)v);
                sv_SetStatusText(text);
                sb_PlaySample(0, (char *)(unsigned)"RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
            }
            m = d2_GetObjValue(row[4], 0);
            d2_SetObjValue(row[4], 0, row[0xc], m - n);
        }
    }
    row = (int *)(unsigned)&full_data_6f4f60;
    do {
        if (row[4] != -1 && d2_GetObjValue(row[4]) != 0) {
            n = d2_GetObjValue(row[4]);
            if (param_5 == 0)
                n = gm_Unk50acf0(rec, row[0]);
            else
                n = gm_Unk50ad70(param_5, row[0], n);
            if (n > 0) {
                if (param_4 == 0)
                    v = 0.0f;
                else
                    v = gm_GoodsPrice(row[0], param_7) * *(float *)(param_3 + 0x49);
                if (sv_CheckGoodAmount((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c), (int)param_7) != 0 &&
                    row[0xc] >= 0) {
                    /* ROUGH: the same limit / confirmation block as above */
                    cm_SendExSellObject(param_1, *(int *)(param_2 + 2), row[0], n, param_7, (int)v);
                    m = d2_GetObjValue(row[4], 0);
                    d2_SetObjValue(row[4], 0, row[0xc], m - n);
                }
            }
        }
        row += 0xe;
    } while ((int)row <= (unsigned)((unsigned)full_data_6f4f60 + 0x37f));
    cm_EndSendGroup();
    while (cm_SendGroupState() == 0)
        sv_PumpNet();
    return 1;
}
