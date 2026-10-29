/* 0x596120, the perfume / mixing contact listener (388 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with
   0x200 SAMMELN (0x14401b4, 0x16) -> 0x595480 (param_1, param_2) and PRODUKTION_MISCHEN (0x1442090, 0x13) ->
   0x58bd60; with 0x400 MEISTERBRIEF (0x143cbc8, 0x16) -> 0x5c3200, PERSONALBUCH (0x143c9f0, 0xc) -> 0x5aab80,
   LAGER (0x1441f90, 0xe) -> 0x5b26b0 (param_1, param_1 + 0x5d), TRANSPORT (0x1441fa0, 0x15) -> 0x5af1a0;
   0x58f510 every frame */

void ai_Unk596120(int param_1, int param_2)
{
    int r;
    int sammeln;
    int mischen;
    int meister;
    int personal;
    int lager;
    int transport;

    sammeln = 0;
    mischen = 0;
    meister = 0;
    personal = 0;
    lager = 0;
    transport = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk596120, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            sammeln = sv_RegisterNamePlate("contact_SAMMELN", U_I((unsigned)&ai_ContactTextRawMaterialSearch), 0x16);
            mischen = sv_RegisterNamePlate("contact_PRODUKTION_MISCHEN", U_I((unsigned)&full_data_1442090), 0x13);
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
        }
        ai_MarketContact((AIBLD *)param_1);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == mischen)
                ai_ProductionWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == personal)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == sammeln)
                ai_TradeSearchWindow((unsigned char *)param_1, param_2);
            else if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
        }
        r = sv_RunFrame(ai_Unk596120, 0x67fff);
    }
}
