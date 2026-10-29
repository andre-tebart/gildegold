/* 0x593ca0, the stone-kontor contact listener (468 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); while the
   flag 0x6f231c has 0x200: LAGER (0x1441f90, 0xe), ABBAU (0x14420a8, 10), SUCHEN_STEIN (0x14420d8, 0x16),
   PRODUKTION (0x1441f98, 0x13), TRANSPORT (0x1441fa0, 0x15), GELAGE (0x1441f88, 0x16), and with the records
   0x4ffe90 (0x35 / 0x34) STROHPUPPE / ZIELSCHEIBE (0x1440ad0 / 0x1440acc, 0x17); 0x5912c0 every frame; the
   dispatcher: LAGER -> 0x5b26b0 (param_1, param_1 + 0x5d), GELAGE -> 0x5b9f30, ABBAU -> 0x592b90,
   SUCHEN_STEIN -> 0x5923c0, TRANSPORT -> 0x5af1a0, PRODUKTION -> 0x593150, ZIEL / STROH -> 0x5bba00 */

void ai_Unk593ca0(int param_1)
{
    int r;
    int lager;
    int abbau;
    int stein;
    int produktion;
    int transport;
    int gelage;
    int stroh;
    int ziel;

    lager = 0;
    abbau = 0;
    stein = 0;
    produktion = 0;
    transport = 0;
    gelage = 0;
    stroh = 0;
    ziel = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk593ca0, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            abbau = sv_RegisterNamePlate("contact_ABBAU", U_I((unsigned)&full_data_14420a8), 10);
            stein = sv_RegisterNamePlate("contact_SUCHEN_STEIN", U_I((unsigned)&full_data_14420d8), 0x16);
            produktion = sv_RegisterNamePlate("contact_PRODUKTION", U_I((unsigned)&full_data_1441f98), 0x13);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            gelage = sv_RegisterNamePlate("contact_GELAGE", U_I((unsigned)&full_data_1441f88), 0x16);
            if (gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x35) != 0)
                stroh = sv_RegisterNamePlate("ob_STROHPUPPE", U_I((unsigned)&full_data_1440ad0), 0x17);
            if (gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x34) != 0)
                ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", U_I((unsigned)&full_data_1440acc), 0x17);
        }
        ai_Unk5912c0();
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == gelage)
                ai_Unk5b9f30(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == abbau)
                ai_Unk592b90(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == stein)
                ai_Unk5923c0(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == produktion)
                ai_Unk593150(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ziel || U_I((unsigned)&ai_ClickedObject) == stroh)
                ai_Unk5bba00(param_1);
        }
        r = sv_RunFrame(ai_Unk593ca0, 0x67fff);
    }
}
