/* 0x593960, the kontor contact listener (407 B): 0x576040 (0x1b, param_1, 0, 0); 0x52e190, the pump 0x5309a0
   (this, 0x67fff); while the flag 0x6f231c has 0x200 the contacts are fetched: PRODUKTION_METALL (0x1441f98,
   0x13) -> 0x593150, TRANSPORT (0x1441fa0, 0x15) -> 0x5af1a0, LAGER (0x1441f90, 0xe) -> 0x5b26b0 (param_1, 0),
   GELAGE (0x1441f88, 0x16) -> 0x5b9f30, and with the records 0x4ffe90 (0x35 / 0x34) STROHPUPPE (0x1440ad0) /
   ZIELSCHEIBE (0x1440acc) -> 0x5bba00; 0x5912c0 runs every frame; the exit 0x576040 (0x1c, param_1, 0, 0) */

void ai_Unk593960(int param_1)
{
    int r;
    int metall;
    int transport;
    int lager;
    int gelage;
    int stroh;
    int ziel;

    metall = 0;
    transport = 0;
    lager = 0;
    gelage = 0;
    stroh = 0;
    ziel = 0;
    ai_Unk576040(0x1b, param_1, 0, 0);
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk593960, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            metall = sv_RegisterNamePlate("contact_PRODUKTION_METALL", U_I((unsigned)&full_data_1441f98), 0x13);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            gelage = sv_RegisterNamePlate("contact_GELAGE", U_I((unsigned)&full_data_1441f88), 0x16);
            if (gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x35) != 0)
                stroh = sv_RegisterNamePlate("ob_STROHPUPPE", U_I((unsigned)&full_data_1440ad0), 0x17);
            if (gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x34) != 0)
                ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", U_I((unsigned)&full_data_1440acc), 0x17);
        }
        ai_Unk5912c0();
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, 0);
            else if (U_I((unsigned)&ai_ClickedObject) == metall)
                ai_Unk593150(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == gelage)
                ai_Unk5b9f30(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ziel || U_I((unsigned)&ai_ClickedObject) == stroh)
                ai_Unk5bba00(param_1);
        }
        r = sv_RunFrame(ai_Unk593960, 0x67fff);
    }
    ai_Unk576040(0x1c, param_1, 0, 0);
}
