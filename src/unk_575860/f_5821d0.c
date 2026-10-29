/* 0x5821d0, the contact dispatcher (467 B): arg 1 = the record, arg 2 = the window's entry list. 0x52e190 once,
   then the pump loop 0x5309a0 (this dispatcher, 0x67fff): the escape 0x14673a0 -> 0x6f22a0 = 1; the contact
   flags 0x6f231c bit 0x200 -> the ids 0x52e1c0 ("contact_REZEPTBUCH" 0x62a4a8 / "contact_PRODUKTION_ALCHIMIST"
   0x62a488, the texts 0x144a2f8 / 0x1441f98, 0x16 / 0x13), bit 0x400 -> the ids "contact_CONTRACT" 0x62a474,
   "contact_LAGER" 0x62a464, "contact_TRANSPORT" 0x62a450, "contact_ALCHIMISTENVERTRAG" 0x62a434 (the texts
   0x144a2a0 / 0x1441f90 / 0x1441fa0 / 0x144a29c, 0xe / 0xe / 0x15 / 0x16); the clicked contact 0x6f22e4:
   TRANSPORT -> 0x5af1a0 (record); LAGER -> 0x5b26b0 (record, the record + 0x5d); the entry list +0x1c -> 0x5823b0
   (record, the list); PRODUCTION -> 0x4eb180 (record, &local, &local2, 0): 0 -> 0x58bd60 (record), else
   0x521150 (0x144a870 + the answer * 4, 0x100); REZEPTBUCH -> 0x57c690 (record); ALCHIMISTENVERTRAG ->
   0x57cf90 (record); CONTRACT -> 0x57bf00 (record) */

void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate(); int ai_OpenTransport(); int ai_StorageWindow();
int ai_ProductionWindow(); void ai_Unk5823b0(); int ai_Unk57c690(); int ai_Unk57cf90();
int ai_Unk57bf00(void *rec);

void ai_Unk5821d0(char *rec, char *list)
{
    int c1;
    int c2;
    int c3;
    int c4;
    int c5;
    int c6;
    int i;
    char a[4];
    char b[4];

    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    c5 = 0;
    c6 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5821d0, 0x67fff) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c6 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4a8, (char *)U_I((unsigned)&full_data_144a2f8), 0x16);
            c5 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a488, (char *)U_I((unsigned)&full_data_1441f98), 0x13);
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            c1 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a474, (char *)U_I((unsigned)&full_data_144a2a0), 0xe);
            c2 = sv_RegisterNamePlate((char *)(unsigned)"contact_LAGER", (char *)U_I((unsigned)&ai_ContactTextStorage), 0xe);
            c3 = sv_RegisterNamePlate((char *)(unsigned)"contact_TRANSPORT", (char *)U_I((unsigned)&ai_ContactTextTransport), 0x15);
            c4 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a434, (char *)U_I((unsigned)&full_data_144a29c), 0x16);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == c3) {
                ai_OpenTransport(rec);
            } else if (U_I((unsigned)&ai_ClickedObject) == c2) {
                ai_StorageWindow(rec, U_I(rec + 0x5d));
            } else if (U_I(list + 0x1c) == 0) {
                if (U_I((unsigned)&ai_ClickedObject) == c5) {
                    i = ahm_ResearchState(rec, a, b, 0);
                    if (i == 0)
                        ai_ProductionWindow(rec);
                    else
                        sv_MessageBox(U_I(((unsigned)&ai_Unk144a80c + 0x64) + i * 4), 0x100);
                } else if (U_I((unsigned)&ai_ClickedObject) == c6) {
                    ai_Unk57c690(rec);
                } else if (U_I((unsigned)&ai_ClickedObject) == c4) {
                    ai_Unk57cf90(rec);
                } else if (U_I((unsigned)&ai_ClickedObject) == c1) {
                    ai_Unk57bf00(rec);
                }
            } else {
                ai_Unk5823b0(rec, list);
            }
        }
    }
}
