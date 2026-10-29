/* 0x5a67a0, the city watch contact listener (376 B, a room of the building driver 0x59e420): 0x52e190, the
   pump 0x5309a0 (this, 0x67fff); with the mask 0x200 the contacts contact_PATROL (0x1440b5c, 0x16),
   ob_WACHPLAN (0x1440b78, 0x16), ob_PERSONALBUCH (0x143c9f0, 0xc), ob_MEISTERBRIEF (0x143cbc8, 0x16) and the
   customs box ob_ZOLLKASSE / ob_ZOLLKASSE_MIT_GEHEIMFACH (0x1440b84, 0x16) by the 0x4ffe90 (the context +0x5d,
   2, 6, 0, 0x147 / 0x148) records; the pending click 0x6f22e4 opens 0x5aab80 (param_1, the name) / 0x5a5f60 /
   0x5a6480 / 0x5a6720 / 0x5c3200 */

void ai_Unk5a67a0(int param_1)
{
    int patrol;
    int wachplan;
    int personal;
    int meister;
    int zoll = 0;
    int r;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5a67a0, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            patrol = sv_RegisterNamePlate("contact_PATROL", U_I((unsigned)&full_data_1440b5c), 0x16);
            wachplan = sv_RegisterNamePlate("ob_WACHPLAN", U_I((unsigned)&full_data_1440b78), 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
            if (gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0x147) != 0)
                zoll = sv_RegisterNamePlate("ob_ZOLLKASSE", U_I((unsigned)&full_data_1440b84), 0x16);
            else if (gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0x148) != 0)
                zoll = sv_RegisterNamePlate("ob_ZOLLKASSE_MIT_GEHEIMFACH", U_I((unsigned)&full_data_1440b84), 0x16);
            if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) && U_I((unsigned)&ai_ClickedObject) != 0) {
                if (U_I((unsigned)&ai_ClickedObject) == personal)
                    ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
                else if (U_I((unsigned)&ai_ClickedObject) == patrol)
                    ai_Unk5a5f60(param_1);
                else if (U_I((unsigned)&ai_ClickedObject) == wachplan)
                    ai_Unk5a6480(param_1);
                else if (U_I((unsigned)&ai_ClickedObject) == zoll)
                    ai_Unk5a6720(param_1);
                else if (U_I((unsigned)&ai_ClickedObject) == meister)
                    ai_MasterLetterWindow(param_1);
            }
        }
        r = sv_RunFrame(ai_Unk5a67a0, 0x67fff);
    }
}
