/* 0x593b00, the wood-kontor contact listener (416 B): 0x52e190; the context *(param_1 + 0x71); the pump
   0x5309a0 (this, 0x67fff); while the flag 0x6f231c has 0x200: LAGER (0x1441f90, 0xe), PRODUKTION_HOLZ
   (0x1442128, 0x13), GELAGE (0x1441f88, 0x16), TRANSPORT (0x1441fa0, 0x15), ZIELSCHEIBE (0x1440acc, 0xc),
   STROHPUPPE (0x1440ad0, 0xc); 0x5912c0 and 0x5b9db0 (param_1, the context) every frame; the dispatcher:
   TRANSPORT -> 0x5af1a0, LAGER -> 0x5b26b0 (param_1, param_1 + 0x5d), HOLZ -> 0x593150, GELAGE -> 0x5b9f30,
   ZIEL / STROH -> 0x5bba00; the window 0x623898 -> 0x5b9730 */


void ai_Unk593b00(int param_1)
{
    int r;
    int lager;
    int holz;
    int gelage;
    int transport;
    int ziel;
    int stroh;
    int ctx;

    lager = 0;
    holz = 0;
    gelage = 0;
    transport = 0;
    ziel = 0;
    stroh = 0;
    sv_ReleaseNamePlates();
    ctx = *(int *)(param_1 + 0x71);
    r = sv_RunFrame(ai_Unk593b00, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            holz = sv_RegisterNamePlate("contact_PRODUKTION_HOLZ", U_I((unsigned)&full_data_1442128), 0x13);
            gelage = sv_RegisterNamePlate("contact_GELAGE", U_I((unsigned)&full_data_1441f88), 0x16);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", U_I((unsigned)&full_data_1440acc), 0xc);
            stroh = sv_RegisterNamePlate("ob_STROHPUPPE", U_I((unsigned)&full_data_1440ad0), 0xc);
        }
        ai_TradeContactChecks();
        ai_Unk5b9db0(param_1, ctx);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == holz)
                ai_Unk593150(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == gelage)
                ai_Unk5b9f30(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ziel || U_I((unsigned)&ai_ClickedObject) == stroh)
                ai_Unk5bba00(param_1);
        }
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&ai_Unk623898))
            ai_Unk5b9730(param_1);
        r = sv_RunFrame(ai_Unk593b00, 0x67fff);
    }
}
