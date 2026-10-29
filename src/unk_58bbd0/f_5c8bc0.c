/* 0x5c8bc0, the medicus privilege handler (803 B, ROUGH): a message record (param_1, the AIMSG by word
   offsets: +0 the id, +2 the type, +4 the handle, +0x24 the amount, +0x194 the choice count). Types 6 / 7
   open the window "privillegien_medicus" 0x62d494 with the two buttons 0x515960 (0x1d3b, 2, the price
   (int)0x50a640 (id)) / 0x475b80; a click on the first tests 0x520f90 (2) and 0x521040, closes 0x4c5f60
   (-2) and writes the text 0x5176d0 (0x5f8318, 0x1d3c / 0x1d3d / 0x1d3e); 0x42ec00 (2) == 0 copies the
   first text, else pays 0x4c47e0 (-1, handle, 0x6f1bc5, price), clears with 0x505840, waits on 0x4c3ae0
   (0x4c4a60 (handle, 0x1c8, 4, 0x40, 0)) with the pump 0x52fe00, shows 0x521150 and sets 0x6f22a0; the
   return 0x81. The other types pay 0x4c47e0 when the amount > 1 and return 0x11, else 0; the pump
   0x5309a0 and the end 0x4758f0. ROUGH: the strcpy blocks of the pseudocode are written as strcpy. */

int gm_Unk505840(char *c);   /* 0x505840 (defined in f3gm.c) */

int ai_Unk5c8bc0(unsigned short *param_1)
{
    char buf[256];
    char text[1024];
    int ret;
    int win;
    int btn1;
    int btn2;
    int t;
    int absv;
    int v;
    float f;

    ret = 2;
    if (*(char *)((char *)param_1 + 2) != 6 && *(char *)((char *)param_1 + 2) != 7) {
        if (*(int *)((char *)param_1 + 0x194) > 1) {
            absv = *(int *)((char *)param_1 + 0x24);
            if (absv < 0)
                absv = -absv;
            f = ((float (*)(unsigned short))gm_GetWorth)(*param_1);   /* the price */
            cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffffe);
            cm_SendSetField(*(int *)((char *)param_1 + 4), 0x1c8, 4, 0x40, 0);
            if (util_Random(2) != 0) {
                cm_SendMoveGoods(0xffffffff, *(int *)((char *)param_1 + 4), *(int *)(unsigned)&ai_OwnPlayer, (int)f);
                gm_AddHealth(*param_1, absv / 2);
                gm_Unk505840((char *)param_1);
            }
            return 0x11;
        }
        return 0;
    }
    f = ((float (*)(unsigned short))gm_GetWorth)(*param_1);   /* the price */
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d494);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    t = io_PrintText(0x1d3b, 2, (int)f);
    btn1 = d2_GetWinObj(win, 1, t);
    btn2 = d2_GetWinObj(win, 1, t + 1);
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&ai_Unk5fe710 == btn1) {
                if (sv_CheckMoney(param_1, 2) != 0 &&
                    sv_CheckGoodAmount(param_1, (int)f, *(int *)(unsigned)&ai_OwnPlayer) != 0) {
                    cm_SendMoneyDelta(*(int *)((char *)param_1 + 4), 0xfffffffe);
                    txt_Format(text, (char *)0x1d3c);
                    if (util_Random(2) == 0) {
                        txt_Format(buf, "%s", 0x1d3e);
                        strcpy(text, buf);
                    } else {
                        txt_Format(buf, "%s", 0x1d3d);
                        strcpy(text, buf);
                        cm_SendMoveGoods(0xffffffff, *(int *)((char *)param_1 + 4), *(int *)(unsigned)&ai_OwnPlayer,
                                     (int)f);
                        gm_Unk505840((char *)param_1);
                    }
                    btn1 = -1;
                    btn2 = -1;
                    ret = 0x81;
                    v = cm_SendSetField(*(int *)((char *)param_1 + 4), 0x1c8, 4, 0x40, 0);
                    while (cm_GetSlotState(v) == 0)
                        sv_PumpNet();
                    sv_MessageBox(text, 0);
                    *(int *)(unsigned)&sv_PopRequest = 1;
                }
            } else if (*(int *)(unsigned)&ai_Unk5fe710 == btn2) {
                *(int *)(unsigned)&sv_PopRequest = 1;
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20))
            *(int *)(unsigned)&sv_PopRequest = 1;
        if (sv_RunFrame(ai_Unk5c8bc0, 0x657c7) == 0) {
            d2_CloseView(win);
            return ret;
        }
    } while (1);
}
