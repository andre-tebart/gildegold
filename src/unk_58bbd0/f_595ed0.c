/* 0x595ed0, the perfumery contact listener (388 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with 0x200:
   SAMMELN (0x14401b4, 0x16) -> 0x595480 (param_1, param_2) and PRODUKTION_PARFUMERIE (0x1442098, 0x13) ->
   0x58bd60; with 0x400: LAGER (0x1441f90) -> 0x5b26b0 (param_1, param_1 + 0x5d), TRANSPORT (0x1441fa0) ->
   0x5af1a0, MEISTERBRIEF (0x143cbc8, 0x16) -> 0x5c3200, PERSONALBUCH (0x143c9f0, 0xc) -> 0x5aab80; 0x58f510
   every frame */

void ai_PerfumeryContact(int building, int room)
{
    int r;
    int sammeln;
    int parfum;
    int lager;
    int transport;
    int meister;
    int personal;

    sammeln = 0;
    parfum = 0;
    lager = 0;
    transport = 0;
    meister = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_PerfumeryContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            sammeln = sv_RegisterNamePlate("contact_SAMMELN", ai_ContactTextRawMaterialSearch, 0x16);
            parfum = sv_RegisterNamePlate("contact_PRODUKTION_PARFUMERIE", ai_ContactTextProductionPerfumery, 0x13);
        }
        if (ai_BuildingPerms & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
        }
        ai_MarketContact((AIBLD *)building);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == parfum)
                ai_ProductionWindow((unsigned char *)building);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport(building);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow(building, ((AIBLD *)building)->objList);
            else if (ai_ClickedObject == sammeln)
                ai_TradeSearchWindow((unsigned char *)building, room);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((unsigned char *)building);
        }
        r = sv_RunFrame(ai_PerfumeryContact, 0x67fff);
    }
}
