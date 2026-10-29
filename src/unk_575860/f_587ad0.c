/* 0x587ad0, the graveyard fog / vermin contact dispatcher (152 B): rec = the record. The contact list 0x52e190
   (); the pump loop 0x5309a0 (this dispatcher, 0x67fff): with the filter 0x6f231c bit 0x200 the contacts
   0x62a88c (U_I (0x144a69c), 0x28) and 0x62a870 (U_I (0x144a490), 0x13) via 0x52e1c0 (name, text, kind); the
   clicked contact 0x6f22e4 (not 0): the first -> 0x587810 (rec), the second -> 0x585380 (rec) */

void ai_Unk587ad0(char *rec)
{
    int c0, c1;

    c0 = 0;
    c1 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk587ad0, 0x67fff) != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c0 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a88c, U_I((unsigned)&full_data_144a69c), 0x28);
            c1 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a870, U_I((unsigned)&full_data_144a490), 0x13);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == c0)
                ai_Unk587810(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c1)
                ai_Unk585380(rec);
        }
    }
}
