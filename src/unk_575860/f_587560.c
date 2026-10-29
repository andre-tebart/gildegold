/* 0x587560, the graveyard trade-contact dispatcher (317 B): rec = the record (+0x5d the owner record). The
   contact list 0x52e190 (); the pump loop 0x5309a0 (this dispatcher, 0x67fff): each frame 0x587c90 (rec); with
   the filter 0x6f231c bit 0x200 the contacts 0x62a818 (U_I (0x144a714), 0x13), "contact_TRANSPORT" 0x62a450
   (U_I (0x1441fa0), 0x15), 0x62a808 (U_I (0x143cbc8), 0xc), 0x62a7f8 (U_I (0x143c9f0), 0xc) and 0x62a464
   (U_I (0x1441f90), 0xe) via 0x52e1c0 (name, text, kind), then 0x5867c0 (rec); the clicked contact 0x6f22e4:
   the first -> 0x58bd60 (rec), the second -> 0x5af1a0 (rec), the third -> 0x5c3200 (rec), the fourth ->
   0x5ad150 (rec, "contact_TRANSPORT") and the fifth -> 0x5b26b0 (rec, *(int *)(rec + 0x5d)) */

int ai_ProductionWindow(); int ai_OpenTransport(); void ai_MasterLetterWindow(); int ai_Unk5ad150(); int ai_StorageWindow();

void ai_Unk587560(char *rec)
{
    int c0, c1, c2, c3, c4;

    c0 = 0;
    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk587560, 0x67fff) != 0) {
        ai_Unk587c90(rec);
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c0 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a818, U_I((unsigned)&full_data_144a714), 0x13);
            c1 = sv_RegisterNamePlate((char *)(unsigned)"contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            c2 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a808, U_I((unsigned)&ai_ContactTextMasterLetter), 0xc);
            c3 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7f8, U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            c4 = sv_RegisterNamePlate((char *)(unsigned)"contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            ai_Unk5867c0(rec);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == c0)
                ai_ProductionWindow(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c1)
                ai_OpenTransport(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c2)
                ai_MasterLetterWindow(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c3)
                ai_Unk5ad150(rec, (char *)(unsigned)&full_data_62a7f8);
            else if (U_I((unsigned)&ai_ClickedObject) == c4)
                ai_StorageWindow(rec, *(__int32 *)(rec + 0x5d));
        }
    }
}
