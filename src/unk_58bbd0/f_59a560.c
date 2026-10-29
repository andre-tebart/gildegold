/* 0x59a560, the fair / market hall contact listener (432 B): the two table objects "ob_KARTENTISCH_germ" and
   "ob_KARTENTISCH" (0x449690, 0xc0) are hidden (0x447300, 0), then the one that 0x5190a0 resolves (the object
   of the current scene, else the first of the two) is shown; the pump 0x5309a0 (0x59a560, 0x67fff), the contact
   list 0x58f510 (param_1); with the flags 0x6f231c & 0x200 the five lookups (transport 0x15, lager 0xe, the two
   objects, class 0x16); the clicked contact 0x6f22e4 opens 0x5af1a0 (param_1), 0x5b26b0 (param_1, param_1 +
   0x5d), 0x5c3200 (param_1) or the town map 0x59a3c0 (the table object's click flag +0x224 off / on) */

void ai_Unk59a560(int param_1, int room)
{
    int obj;
    int transport = 0;
    int lager = 0;
    int meister = 0;
    int kartentisch = 0;

    obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH_germ");
    if (obj != 0)
        d3if_ShowTree(obj, 0);
    obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH");
    if (obj != 0)
        d3if_ShowTree(obj, 0);
    obj = ls_GetLanguage();
    if (obj == 0)
        obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH_germ");
    if (obj == 0)
        obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH");
    if (obj != 0)
        d3if_ShowTree(obj, 1);
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk59a560, 0x67fff) != 0) {
        ai_MarketContact((AIBLD *)param_1);
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
            kartentisch = sv_RegisterNamePlate("ob_KARTENTISCH", U_I((unsigned)&full_data_1440a00), 0x16);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == kartentisch) {
                U_I(kartentisch + 0x224) = 0;
                ai_Unk59a3c0();
                U_I(kartentisch + 0x224) = 1;
            }
        }
    }
}
