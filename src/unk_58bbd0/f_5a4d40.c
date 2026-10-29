/* 0x5a4d40, the thieves' guild listener (522 B, room 0x54 of the building driver 0x59e420): 0x52e190, the
   great sword object 0x449690 (0, 0xc0, 0) shown by 0x447300 only from the crew size byte +0x247 of the kind
   record >= 2, then the pump 0x5309a0 (this, 0x67fff) fetches the contact_EINBRUCH (0x1440944) /
   contact_GEBAEUDE_AUSSPIONIEREN (0x1440994) / contact_ANGRIFF (0x1441fa8, only with the byte >= 2) /
   contact_TASCHENDIEBSTAHL (0x1442018) / ob_INFORMATIONSPERGAMENT (0x143cb38) windows through 0x52e1c0 when
   the mask 0x200 is set, ob_PERSONALBUCH (0x143c9f0) / ob_MEISTERBRIEF (0x143cbc8) when 0x400 is set, and
   dispatches the pending click 0x6f22e4 to the starts 0x5a4130 / 0x5a44e0 / 0x5a4b30, the windows 0x5a4b90 /
   0x5a45c0 and the controls 0x5aab80 / 0x5c3200 */

void ai_Unk5a4d40(int param_1)
{
    int burglary = 0;
    int espionage = 0;
    int angriff = 0;
    int taschen = 0;
    int personal = 0;
    int pergament = 0;
    int meister = 0;
    int obj;
    int click;
    char *krec;

    sv_ReleaseNamePlates();
    krec = (char *)B_REC(*(unsigned char *)param_1);
    if (*(unsigned char *)(krec + 0x247) >= 2) {
        obj = d3_FindObject(0, 0xc0, 0, "ub_GROSSSCHWERT");
        if (obj != 0)
            d3if_ShowTree(obj, 1);
    } else {
        obj = d3_FindObject(0, 0xc0, 0, "ub_GROSSSCHWERT");
        if (obj != 0)
            d3if_ShowTree(obj, 0);
    }
    if (sv_RunFrame(ai_Unk5a4d40, 0x67fff) == 0)
        return;
    do {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            burglary = sv_RegisterNamePlate("contact_EINBRUCH", U_I((unsigned)&full_data_1440944), 0x17);
            espionage = sv_RegisterNamePlate("contact_GEBAEUDE_AUSSPIONIEREN", U_I((unsigned)&full_data_1440994), 0x17);
            if (*(unsigned char *)(krec + 0x247) >= 2)
                angriff = sv_RegisterNamePlate("contact_ANGRIFF", U_I((unsigned)&full_data_1441fa8), 0x17);
            taschen = sv_RegisterNamePlate("contact_TASCHENDIEBSTAHL", U_I((unsigned)&full_data_1442018), 0x17);
            pergament = sv_RegisterNamePlate("ob_INFORMATIONSPERGAMENT", U_I((unsigned)&full_data_143cb38), 0x16);
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", U_I((unsigned)&ai_ContactTextMasterLetter), 0x16);
        }
        click = U_I((unsigned)&ai_ClickedObject);
        if (click != 0) {
            if (click == burglary)
                ai_Unk5a4130(param_1);
            else if (click == espionage)
                ai_Unk5a44e0((unsigned char *)param_1);
            else if (click == angriff)
                ai_Unk5a4b30(param_1);
            else if (click == taschen)
                ai_Unk5a4b90(param_1);
            else if (click == personal)
                ai_PersonnelBook(param_1, "ob_PERSONALBUCH");
            else if (click == pergament)
                ai_Unk5a45c0(param_1);
            else if (click == meister)
                ai_MasterLetterWindow(param_1);
        }
    } while (sv_RunFrame(ai_Unk5a4d40, 0x67fff) != 0);
}
