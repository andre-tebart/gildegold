/* 0x59df80, the money lender contact listener (288 B): the pump 0x5309a0 (0x59df80, 0x67fff) and the lookups
   "contact_GELDWECHSEL" (0x1440590) / "contact_KREDITE" (0x14404e0) / with the flags 0x6f231c & 0x200
   "contact_MEISTERBRIEF" (0x143cbc8) and "contact_VERMOEGENSUEBERSICHT" (0x1442050); the master letter opens
   0x5c3200, the overview 0x59df10; the player's own building (param_1 + 0x27 == 0x6f25b8) opens 0x59d5e0
   (money exchange) / 0x59bad0 (credits), another player's 0x59c700 / 0x59b160 */

void ai_Unk59df80(int param_1, int room)
{
    int meister = 0;
    int overview = 0;
    int wechsel;
    int kredite;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk59df80, 0x67fff) != 0) {
        wechsel = sv_RegisterNamePlate("contact_GELDWECHSEL", U_I((unsigned)&full_data_1440590), 0x10);
        kredite = sv_RegisterNamePlate("contact_KREDITE", U_I((unsigned)&full_data_14404e0), 0x10);
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
            meister = sv_RegisterNamePlate("contact_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
            overview = sv_RegisterNamePlate("contact_VERMOEGENSUEBERSICHT", U_I((unsigned)&full_data_1442050), 0x16);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == meister)
                ai_MasterLetterWindow(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == overview)
                ai_Unk59df10(param_1);
            if (U_I((unsigned)&ai_ClickedObject) != 0) {
                if (*(unsigned short *)(param_1 + 0x27) == U_W((unsigned)&ai_PlayerId)) {
                    if (U_I((unsigned)&ai_ClickedObject) == wechsel)
                        ai_Unk59d5e0(param_1);
                    else if (U_I((unsigned)&ai_ClickedObject) == kredite)
                        ai_Unk59bad0(param_1);
                } else if (U_I((unsigned)&ai_ClickedObject) == wechsel) {
                    ai_Unk59c700(param_1);
                } else if (U_I((unsigned)&ai_ClickedObject) == kredite) {
                    ai_Unk59b160(param_1);
                }
            }
        }
    }
}
