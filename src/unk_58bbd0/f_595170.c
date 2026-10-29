/* 0x595170, the smithy contact listener (448 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); while the flag
   0x6f231c has 0x200: with 0x576410 (0x20) the smithy contact PRODUKTION_SCHMIEDEN (0x1442108, 0x13) and with
   0x576410 (0x80) MEISTERBRIEF (0x143cbc8, 0x16); with 0x400: LAGER (0x1441f90, 0xe) with 0x576410 (0x10),
   TRANSPORT (0x1441fa0, 0x15) with 0x576410 (0x100), PERSONALBUCH (0x143c9f0, 0xc) with 0x576410 (0x40); the
   dispatcher: LAGER -> 0x5b26b0 (param_1, param_1 + 0x5d), SCHMIEDEN -> 0x58bd60, TRANSPORT -> 0x5af1a0,
   PERSONALBUCH -> 0x5aab80, MEISTERBRIEF -> 0x5c3200; 0x58f510 every frame; 0x52e190 on the exit */

int ai_Unk576410();

void ai_Unk595170(int param_1, int room)
{
    int r;
    int schmieden;
    int meister;
    int lager;
    int transport;
    int personal;

    schmieden = 0;
    meister = 0;
    lager = 0;
    transport = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk595170, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            if (ai_Unk576410(0x20) == 1)
                schmieden = sv_RegisterNamePlate("contact_PRODUKTION_SCHMIEDEN", U_I((unsigned)&full_data_1442108), 0x13);
            else
                schmieden = 0;
            if (ai_Unk576410(0x80) == 1)
                meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
            else
                meister = 0;
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            if (ai_Unk576410(0x10) == 1)
                lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            else
                lager = 0;
            if (ai_Unk576410(0x100) == 1)
                transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            else
                transport = 0;
            if (ai_Unk576410(0x40) == 1)
                personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            else
                personal = 0;
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == schmieden)
                ai_ProductionWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == personal)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
        }
        ai_MarketContact((AIBLD *)param_1);
        r = sv_RunFrame(ai_Unk595170, 0x67fff);
    }
    sv_ReleaseNamePlates();
}
