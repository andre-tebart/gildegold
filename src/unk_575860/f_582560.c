/* 0x582560, the cannon contact dispatcher (159 B): arg 1 = the record, arg 2 = the entry list. 0x52e190 once,
   then the pump loop 0x5309a0 (this dispatcher, 0x67fff): the contact flags 0x6f231c bit 0x400 -> the id
   0x52e1c0 ("contact_KANONE" 0x62a4bc, the text 0x144a334, 0x16); the escape 0x14673a0 -> 0x6f22a0 = 1; the
   clicked contact 0x6f22e4: with the list +0x1c clear and the id KANONE -> 0x57e270 (the cannon scene); with the
   list set -> 0x5823b0 (the record, the list) */

void ai_Unk582560(char *rec, char *list)
{
    int k;
    int id;

    id = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk582560, 0x67fff) != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400)
            id = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4bc, (char *)U_I((unsigned)&full_data_144a334), 0x16);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I(list + 0x1c) == 0) {
                if (U_I((unsigned)&ai_ClickedObject) == id)
                    ai_Unk57e270(rec);
            } else {
                ai_Unk5823b0(rec, list);
            }
        }
    }
}
