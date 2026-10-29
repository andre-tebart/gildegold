/* 0x599cb0, the building production listener "contact_PRODUKTION_BRAUEN" (336 B): the pump 0x5309a0 (0x599cb0,
   0x67fff); the contact lookups are redone only when the flags 0x6f231c change (0x200: the brewing contact,
   0x400: "ob_PERSONALBUCH", "ob_MEISTERBRIEF", "contact_TRANSPORT", "contact_LAGER"); 0x58f510 (param_1) and
   the clicked contact 0x6f22e4 opens 0x5aab80 (param_1, "ob_PERSONALBUCH"), 0x5b26b0 (param_1, param_1 + 0x5d),
   0x5c3200 (param_1), the production window 0x58bd60 (param_1) or the transport window 0x5af1a0 (param_1) */

void ai_Unk599cb0(int param_1, int room)
{
    int brauen;
    int personal;
    int lager;
    int meister;
    int transport;

    brauen = 0;
    personal = 0;
    lager = 0;
    meister = 0;
    transport = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk599cb0, 0x67fff) != 0) {
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0)
            brauen = sv_RegisterNamePlate("contact_PRODUKTION_BRAUEN", U_I((unsigned)&full_data_1442130), 0x2d);
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x400) != 0) {
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
        }
        ai_MarketContact((AIBLD *)param_1);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == personal)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == brauen)
                ai_ProductionWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
        }
    }
}
