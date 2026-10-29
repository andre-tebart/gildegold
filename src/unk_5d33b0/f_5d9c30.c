/* 0x5d9c30, the contact listener of the gypsy production building (348 B): building the building record. The contact
   plates (sv_RegisterNamePlate) are created each frame from the permission bits ai_BuildingPerms: bit 0x200 ->
   contact_ZUTATEN_SUCHEN_ZIGEUNER (0x144aa34, 0x16), contact_TRANSPORT (0x15), contact_PRODUKTION_ZIGEUNER (
   0x1441f98, 0x13), contact_LAGER (0xe) and contact_BETTELN (0x144a954, 0x2c); bit 0x400 -> contact_PERSONALBUCH and
   contact_MEISTERBRIEF. The click (ai_ClickedObject) opens the helpers window 0x584d70 for the ingredient search, the
   transport, the production, the store (second argument 0), the master letter or the begging window
   (ai_GypsyBeggingWindow) of the building; the personnel book is checked after the chain from a fresh read of the click.
   Every frame ends with the trade window checks ai_TradeContactChecks. The pump (this function, 0x67fff) runs between the
   sv_ReleaseNamePlates call and the loop; the loop is entered by the pump result. Void. Reasoned: the plate names are
   the literal strings, the callees are read off the calls (the ingredient search window is the "HANDEL_SUCHEN" helper). */

void ai_GypsyProductionContact(int building)
{
    int zutaten = 0;
    int transport = 0;
    int produktion = 0;
    int lager = 0;
    int betteln;
    int personal;
    int meister;
    int pump;

    personal = 0;
    meister = 0;
    betteln = 0;
    sv_ReleaseNamePlates();
    pump = sv_RunFrame(ai_GypsyProductionContact, 0x67fff);
    while (pump != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            zutaten = sv_RegisterNamePlate("contact_ZUTATEN_SUCHEN_ZIGEUNER", txt_Ptrs[0x421d], 0x16);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            produktion = sv_RegisterNamePlate("contact_PRODUKTION_ZIGEUNER", txt_Ptrs[0x1f76], 0x13);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            betteln = sv_RegisterNamePlate("contact_BETTELN", txt_Ptrs[0x41e5], 0x2c);
        }
        if ((ai_BuildingPerms & 0x400) != 0) {
            personal = sv_RegisterNamePlate("contact_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meister = sv_RegisterNamePlate("contact_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
        }
        if (ai_ClickedObject != 0) {
            if ((ai_BuildingPerms & 0x200) != 0) {
                if (ai_ClickedObject == zutaten)
                    ai_GypsyHelperWindow((char *)building);
                else if (ai_ClickedObject == transport)
                    ai_OpenTransport(building);
                else if (ai_ClickedObject == produktion)
                    ai_ProductionWindow((unsigned char *)building);
                else if (ai_ClickedObject == lager)
                    ai_StorageWindow(building, 0);
                else if (ai_ClickedObject == meister)
                    ai_MasterLetterWindow((unsigned char *)building);
                else if (ai_ClickedObject == betteln)
                    ai_GypsyBeggingWindow(building);
            }
            if (ai_ClickedObject == personal)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
        }
        ai_TradeContactChecks();
        pump = sv_RunFrame(ai_GypsyProductionContact, 0x67fff);
    }
}
