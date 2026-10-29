/* 0x586840, the graveyard contact dispatcher (611 B): the contact list 0x52e190 (), the pump loop 0x5309a0 (this
   dispatcher, 0x67fff): each frame 0x587c90 (rec); with the filter 0x6f231c bit 0x200 the contacts
   "contact_ALTERN_LASSEN" (0x62a7e0, 0x2f), "contact_HEIMSUCHUNG" (0x62a7cc, 0x29), "contact_ZUTATEN_SUCHEN"
   (0x62a7b4, 0x13), "contact_GRAEBER_PFLEGEN" (0x62a79c, 0x2a), "contact_GEHILFEN_ERSCHAFFEN" (0x62a780,
   0x28), "contact_VERSICHERUNG" (0x62a768, 0x2b), "contact_LIEGEPLAN" (0x62a754, 0x13) and "contact_TRANSPORT"
   (0x62a450, 0x15) via 0x52e1c0 (name, the text U_I (0x144a440 / 410 / 65c / 634 / 680 / 590 / 5d0 / 1441fa0),
   the kind); always "contact_GRAB_MIETEN" (0x62a740, U_I (0x144a5d4), 0x16). No clicked contact 0x6f22e4: the
   gate 0x6f2448 and the key 0x1468410 -> 0x1e 0x586e40, 0x2f 0x587370, 0x22 0x585380, 0x23 0x586ab0, 0x19
   0x587300, 0x16 0x5871e0, 0x11 0x587340. Else: with the bit 0x200, 0x5867c0 (rec) first, then the matching
   contact: ALTERN -> 0x586e40, HEIMSUCHUNG -> 0x586ab0, ZUTATEN -> 0x584d70, GRAEBER -> 0x587300, GEHILFEN ->
   0x587340, VERSICHERUNG -> 0x587370, LIEGEPLAN -> 0x5871c0, TRANSPORT -> 0x5af1a0; with the byte 0x6f231c
   negative and GRAB_MIETEN clicked -> 0x5871e0 (rec); 0x5912c0 () each frame */

void sv_ReleaseNamePlates(void); void ai_Unk587c90(); int sv_RegisterNamePlate(); int ai_TradeContactChecks();
void ai_Unk586e40(); void ai_Unk587370(); void ai_Unk586ab0(); void ai_Unk587300();
void ai_Unk5871e0(); void ai_Unk587340(); void ai_Unk5871c0();

void ai_Unk586840(char *rec)
{
    int c_altern, c_heim, c_zut, c_graeber, c_gehilfen, c_vers, c_liege, c_trans, c_mieten;

    c_altern = 0;
    c_heim = 0;
    c_zut = 0;
    c_graeber = 0;
    c_gehilfen = 0;
    c_vers = 0;
    c_liege = 0;
    c_trans = 0;
    c_mieten = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk586840, 0x67fff) != 0) {
        ai_Unk587c90(rec);
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c_altern = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7e0, U_I((unsigned)&full_data_144a440), 0x2f);
            c_heim = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7cc, U_I((unsigned)&full_data_144a410), 0x29);
            c_zut = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7b4, U_I((unsigned)&full_data_144a65c), 0x13);
            c_graeber = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a79c, U_I((unsigned)&full_data_144a634), 0x2a);
            c_gehilfen = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a780, U_I((unsigned)&full_data_144a680), 0x28);
            c_vers = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a768, U_I((unsigned)&full_data_144a590), 0x2b);
            c_liege = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a754, U_I((unsigned)&full_data_144a5d0), 0x13);
            c_trans = sv_RegisterNamePlate((char *)(unsigned)"contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
        }
        c_mieten = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a740, U_I((unsigned)&full_data_144a5d4), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) == 0) {
            if (U_I((unsigned)&ai_Unk6f2448) != 0) {
                switch (U_I((unsigned)&kb_Last)) {
                case 0x1e: ai_Unk586e40(rec); break;
                case 0x2f: ai_Unk587370(rec); break;
                case 0x22: ai_Unk585380(rec); break;
                case 0x23: ai_Unk586ab0(rec); break;
                case 0x19: ai_Unk587300(rec); break;
                case 0x16: ai_Unk5871e0(rec); break;
                case 0x11: ai_Unk587340(rec); break;
                }
            }
        } else {
            if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
                ai_Unk5867c0(rec);
                if (U_I((unsigned)&ai_ClickedObject) == c_altern)
                    ai_Unk586e40(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_heim)
                    ai_Unk586ab0(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_zut)
                    ai_GypsyHelperWindow(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_graeber)
                    ai_Unk587300(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_gehilfen)
                    ai_Unk587340(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_vers)
                    ai_Unk587370(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_liege)
                    ai_Unk5871c0(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_trans)
                    ai_OpenTransport(rec);
            }
            if ((char)U_I((unsigned)&ai_BuildingPerms) < 0 && U_I((unsigned)&ai_ClickedObject) == c_mieten)
                ai_Unk5871e0(rec);
        }
        ai_TradeContactChecks();
    }
}
