/* 0x5876a0, the graveyard seance contact dispatcher (95 B): rec = the record. The pump loop 0x5309a0 (this
   dispatcher, 0x67fff): the contact 0x62a83c (U_I (0x144a764), 0x16) via 0x52e1c0 (name, text, kind); the
   clicked contact 0x6f22e4 (not 0) on it -> 0x586610 (rec) */

void ai_Unk5876a0(char *rec)
{
    int c;

    while (sv_RunFrame(ai_Unk5876a0, 0x67fff) != 0) {
        c = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a83c, U_I((unsigned)&full_data_144a764), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0 && U_I((unsigned)&ai_ClickedObject) == c)
            ai_Unk586610(rec);
    }
}
