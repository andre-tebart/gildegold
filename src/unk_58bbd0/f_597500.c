/* 0x597500, the master / guild contact listener (407 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with
   0x576410 (8) VERHANDELN (0x14421b8, 0xc) -> 0x5b3130 (param_1, param_2, 0) and HANDWERKSKUNST (0x1442180,
   0xc) -> 0x5b3130 (..., 1); with 0x578170 BEWEISBUCH (0x143feb8, 0xc) -> 0x5b4590, SPIONAGE (0x143fda0, 0x17)
   -> 0x597380, MEISTERPRUEFUNG (0x1442190, 0xc) -> 0x596ff0, SELBSTMORD (0x144ad10, 0x24) -> 0x5973f0; when no
   contact is pending and 0x6f2450 is set in the state '-' the mistress window 0x596ba0 (param_1) opens */

void ai_Unk597500(int param_1, int param_2)
{
    int r;
    int verhandeln;
    int handwerk;
    int beweis;
    int spionage;
    int meister;
    int selbstmord;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk597500, 0x67fff);
    while (r != 0) {
        if (ai_Unk576410(8) == 1) {
            verhandeln = sv_RegisterNamePlate("contact_VERHANDELN", U_I((unsigned)&full_data_14421b8), 0xc);
            handwerk = sv_RegisterNamePlate("contact_HANDWERKSKUNST", U_I((unsigned)&full_data_1442180), 0xc);
        } else {
            verhandeln = 0;
            handwerk = 0;
        }
        if (ai_Unk578170() == 1) {
            beweis = sv_RegisterNamePlate("contact_BEWEISBUCH", U_I((unsigned)&full_data_143feb8), 0xc);
            spionage = sv_RegisterNamePlate("contact_SPIONAGE", U_I((unsigned)&full_data_143fda0), 0x17);
            meister = sv_RegisterNamePlate("contact_MEISTERPRUEFUNG", U_I((unsigned)&full_data_1442190), 0xc);
            selbstmord = sv_RegisterNamePlate("contact_SELBSTMORD", U_I((unsigned)&full_data_144ad10), 0x24);
        } else {
            beweis = 0;
            spionage = 0;
            meister = 0;
            selbstmord = 0;
        }
        if (U_I((unsigned)&ai_ClickedObject) == 0) {
            if (U_I((unsigned)&full_data_6f2450) != 0 && U_I((unsigned)&kb_Last) == '-')
                ai_MistressWindow(param_1);
        } else if (U_I((unsigned)&ai_ClickedObject) == verhandeln) {
            ai_Unk5b3130(param_1, param_2, 0);
        } else if (U_I((unsigned)&ai_ClickedObject) == handwerk) {
            ai_Unk5b3130(param_1, param_2, 1);
        } else if (U_I((unsigned)&ai_ClickedObject) == beweis) {
            ai_Unk5b4590(param_1);
        } else if (U_I((unsigned)&ai_ClickedObject) == meister) {
            ai_Unk596ff0();
        } else if (U_I((unsigned)&ai_ClickedObject) == spionage) {
            ai_Unk597380();
        } else if (U_I((unsigned)&ai_ClickedObject) == selbstmord) {
            ai_Unk5973f0();
        }
        r = sv_RunFrame(ai_Unk597500, 0x67fff);
    }
}
