/* 0x59a710, leaves the market of the building 0x6f1bc8 with the record param_2 (240 B): 0x75ec60 / 0x6f2304 =
   0, 0x529fb0 / 0x52ae40, 0x6f2318 = 1, the window 0x623860 hidden (0x475590); 0x6f2310 = the person
   0x4ffe90 (building + 0x5d, 1, 0, param_2); the event 0x576040 (0x17, person, 0, param_2) and the loop
   0x5af1d0 (param_1, 9) while the person exists (0x6f2318 = 1); then 0x576040 (0x18, ..), 0x576040 (0x17,
   person, 1, ..), the person cleared and 0x475590 () */

void ai_Unk59a710(int param_1, int param_2)
{
    U_I((unsigned)&ai_Unk75ec60) = 0;
    U_I((unsigned)&ai_Unk6f2304) = 0;
    sv_Unk529fb0();
    sv_ClearSelection();
    U_I((unsigned)&ai_Unk6f2318) = 1;
    if (U_I((unsigned)&sv_PanelForm) != -1)
        d2_SetViewEnabled(U_I((unsigned)&sv_PanelForm), 0);
    U_I((unsigned)&sv_PanelShown) = gm_Query(U_I(U_I((unsigned)&ai_MarketBuilding) + 0x5d), 1, 0, param_2);
    if (U_I((unsigned)&sv_PanelShown) != 0) {
        ai_Unk576040(0x17, U_I((unsigned)&sv_PanelShown), 0, param_2);
        while (U_I((unsigned)&sv_PanelShown) != 0) {
            U_I((unsigned)&ai_Unk6f2318) = 1;
            ai_TransportWindow(param_1, 9);
        }
    }
    ai_Unk576040(0x18, U_I((unsigned)&sv_PanelShown), 0, param_2);
    ai_Unk576040(0x17, U_I((unsigned)&sv_PanelShown), 1, param_2);
    U_I((unsigned)&sv_PanelShown) = 0;
    if (U_I((unsigned)&sv_PanelForm) != -1)
        d2_SetViewEnabled();
}
