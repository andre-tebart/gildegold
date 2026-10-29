/* 0x5a36c0, the production building listener (330 B): the pump 0x5309a0 (this, 0x67fff) runs 0x58f510
   (param_1) each frame and, while 0x6f231c bit 0x200 is set, looks up "contact_PRODUKTION_SCHREIBEN"
   (0x1442078, class 0x13), "contact_LAGER" (0x1441f90, class 0xe), "contact_TRANSPORT" (0x1441fa0, class
   0x15), "ob_PERSONALBUCH" (0x143c9f0, class 0xc) and "ob_MEISTERBRIEF" (0x143cbc8, class 0xc); a click opens
   0x5aab80 (param_1, "ob_PERSONALBUCH"), 0x5b26b0 (param_1, the building +0x5d), 0x58bd60 (the production
   window), 0x5af1a0 or 0x5c3200 */

int ai_PersonnelBook(); int ai_StorageWindow(); int ai_ProductionWindow();
int ai_OpenTransport(); void ai_MasterLetterWindow();

void ai_Unk5a36c0(int param_1)
{
    int schreiben = 0;
    int lager = 0;
    int transport = 0;
    int personalbuch = 0;
    int meisterbrief = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a36c0, 0x67fff) != 0) {
        ai_MarketContact((AIBLD *)param_1);
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
            schreiben = sv_RegisterNamePlate("contact_PRODUKTION_SCHREIBEN", U_I((unsigned)&full_data_1442078), 0x13);
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            personalbuch = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            meisterbrief = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0xc);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0 && (U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == personalbuch)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == schreiben)
                ai_ProductionWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == meisterbrief)
                ai_MasterLetterWindow(param_1);
        }
    }
}
