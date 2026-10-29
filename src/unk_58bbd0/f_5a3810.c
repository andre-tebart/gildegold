/* 0x5a3810, the church listener (412 B): the pump 0x5309a0 (this, 0x67fff); with 0x6f231c bit 0x200 the
   contacts "contact_PREDIGT" (0x14407f4) and "ob_WEIHWASSERBECKEN" (0x1440794, both 0x16); for a building of
   another owner (param_1 + 0x27 neither the player nor 0xffff) the room 0x4ffe90 (param_1 + 0x5d, 2, 6, 0, 0xf2)
   and the contacts "ob_KIRCHE_BEICHTSTUHLRAUM" (0x14407b8), "ob_SPENDENKAESTCHEN" (0x14407d4) and
   "ob_WEIHWASSERBECKEN"; with no click and 0x1468410 == '0' and 0x6f2448 set the confession 0x5a2b60 opens
   itself; the clicks open 0x5a2e60 (sermon), 0x5a23a0 (baptism, param_2), 0x5a2b60 (confession), 0x5a1af0
   (donation) */

void ai_Unk5a3810(int param_1, int param_2)
{
    int predigt = 0;
    int becken = 0;
    int beichtstuhl = 0;
    int spendenkasten = 0;
    int room;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a3810, 0x67fff) != 0) {
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
            predigt = sv_RegisterNamePlate("contact_PREDIGT", U_I((unsigned)&full_data_14407f4), 0x16);
            becken = sv_RegisterNamePlate("ob_WEIHWASSERBECKEN", U_I((unsigned)&full_data_1440794), 0x16);
        }
        if (U_W(param_1 + 0x27) != U_W((unsigned)&ai_PlayerId) && U_W(param_1 + 0x27) != 0xffff) {
            room = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0xf2);
            if (room == 0)
                beichtstuhl = 0;
            else
                beichtstuhl = sv_RegisterNamePlate("ob_KIRCHE_BEICHTSTUHLRAUM", U_I((unsigned)&full_data_14407b8), 0x16);
            spendenkasten = sv_RegisterNamePlate("ob_SPENDENKAESTCHEN", U_I((unsigned)&full_data_14407d4), 0x16);
            becken = sv_RegisterNamePlate("ob_WEIHWASSERBECKEN", U_I((unsigned)&full_data_1440794), 0x16);
        }
        if (U_I((unsigned)&ai_ClickedObject) == 0) {
            if (*(char *)(unsigned)&kb_Last == '0' && U_I((unsigned)&ai_Unk6f2448) != 0)
                ai_Unk5a2b60(param_1);
        } else {
            if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
                if (U_I((unsigned)&ai_ClickedObject) == predigt)
                    ai_Unk5a2e60(param_1);
                else if (U_I((unsigned)&ai_ClickedObject) == becken)
                    ai_Unk5a23a0(param_1, param_2);
            }
            if (U_W(param_1 + 0x27) != U_W((unsigned)&ai_PlayerId)) {
                if (U_I((unsigned)&ai_ClickedObject) == beichtstuhl)
                    ai_Unk5a2b60(param_1);
                if (U_I((unsigned)&ai_ClickedObject) == spendenkasten)
                    ai_Unk5a1af0(param_1);
                if (U_I((unsigned)&ai_ClickedObject) == becken)
                    ai_Unk5a23a0(param_1, param_2);
            }
        }
    }
}
