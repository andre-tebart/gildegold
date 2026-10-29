/* 0x57e0e0, the tutorial skill "use" window (336 B, the 0x57de60 sibling): the run record 0x4eaa50 (record),
   0 for none -> 0; the window "Locations_Domizil_skill" (0x62a2c4) with 0x476f80 (0, 0) / 0x475600, the sound
   0x52ea30 (0x6ed938), the text 0x403d (0x182) and the entries 0x403e / +1 via 0x475b80 (window, 2, text),
   0xf8a668 = -1; the pump loop 0x5309a0 (this handler, 0x677c7): 0x4eaa50 (record) 0 -> 0x6f22a0 = 1, the pump
   0x20007, 0x4758f0 (window), 0; the click 0x14673a0 -> 0x6f22a0 = 1; the first entry -> the result 1,
   0x4eb410 (record) and 0x4ece10 (the run record), 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 ->
   0x4758f0 (window), the result */

void ahm_RescheduleResearch();
int ahm_SendDelete();

int ai_Unk57e0e0(char *rec)
{
    char *rr;
    char *win;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    rr = (char *)ahm_OwnerChar(rec);
    if (!rr)
        return 0;
    win = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x403d, 0x182);
    d2_SetForm(win, 2);
    v = io_PrintText(0x403e);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        rr = (char *)ahm_OwnerChar(rec);
        if (!rr) {
            U_I((unsigned)&sv_PopRequest) = 1;
            sv_RunFrame(ai_Unk57e0e0, 0x20007);
            d2_CloseView(win);
            return 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                done = 1;
                ahm_RescheduleResearch(rec);
                ahm_SendDelete(rr);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk57e0e0, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}
