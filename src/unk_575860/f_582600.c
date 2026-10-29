/* 0x582600, the spy contact dispatcher (289 B): arg 1 = the record, arg 2 = the entry list. 0x52e190 once, then
   the pump loop 0x5309a0 (this dispatcher, 0x67fff): the contact flags 0x6f231c bit 0x400 -> the ids 0x52e1c0
   ("contact_VERGIFTEN" 0x62a524 / "contact_SPIONE_AUSSCHALTEN" 0x62a508 / "contact_SCHLEICHENDE_LAEHMUNG"
   0x62a4e8 / "contact_BEWEISE_FAELSCHEN" 0x62a4cc, the texts 0x144a1a8 / 0x144a22c / 0x144a268 / 0x144a1ec,
   0x16); the escape 0x14673a0 -> 0x6f22a0 = 1; the clicked contact 0x6f22e4, with the list +0x1c clear:
   0x581950 / 0x581020 / 0x581db0 / 0x581400 (the record); with the list set -> 0x5823b0 (the record, the list) */

void ai_Unk582600(char *rec, char *list)
{
    int c1;
    int c2;
    int c3;
    int c4;

    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk582600, 0x67fff) != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            c1 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a524, (char *)U_I((unsigned)&full_data_144a1a8), 0x16);
            c2 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a508, (char *)U_I((unsigned)&full_data_144a22c), 0x16);
            c3 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4e8, (char *)U_I((unsigned)&full_data_144a268), 0x16);
            c4 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4cc, (char *)U_I((unsigned)&full_data_144a1ec), 0x16);
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I(list + 0x1c) == 0) {
                if (U_I((unsigned)&ai_ClickedObject) == c1)
                    ai_Unk581950(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c2)
                    ai_Unk581020(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c3)
                    ai_Unk581db0(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c4)
                    ai_Unk581400(rec);
            } else {
                ai_Unk5823b0(rec, list);
            }
        }
    }
}
