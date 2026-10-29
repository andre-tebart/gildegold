/* 0x595330, the carpenter contact listener (331 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with 0x200 the
   contact PRODUKTION_TISCHLER (0x1442118, 0x13); with 0x400 LAGER (0x1441f90, 0xe), TRANSPORT (0x1441fa0,
   0x15), MEISTERBRIEF (0x143cbc8, 0xc) and PERSONALBUCH (0x143c9f0, 0xc); the dispatcher: TISCHLER ->
   0x58bd60, TRANSPORT -> 0x5af1a0, PERSONALBUCH -> 0x5aab80, LAGER -> 0x5b26b0 (param_1, param_1 + 0x5d),
   MEISTERBRIEF -> 0x5c3200; 0x58f510 every frame */

void ai_Unk595330(int param_1, int room)
{
    int r;
    int tischler;
    int lager;
    int transport;
    int meister;
    int personal;

    tischler = 0;
    lager = 0;
    transport = 0;
    meister = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk595330, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200)
            tischler = sv_RegisterNamePlate("contact_PRODUKTION_TISCHLER", U_I((unsigned)&full_data_1442118), 0x13);
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0xc);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
        }
        ai_MarketContact((AIBLD *)param_1);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == tischler)
                ai_ProductionWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == personal)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
        }
        r = sv_RunFrame(ai_Unk595330, 0x67fff);
    }
}
