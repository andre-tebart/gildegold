/* 0x594e50, the criminal contact listener (643 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); while the flag
   0x6f231c has 0x200: AUSRUESTEN (0x1441fb8, 0xd) -> 0x5bb350, AUF_LAUER_LEGEN (0x1441fb0, 0x15) -> 0x594180,
   ANGRIFF (0x1441fa8, 0x17) -> 0x594470, SCHUTZGELD_ERPRESSEN (0x1442010, 0x17) -> 0x594790,
   GEBAEUDE_AUSSPIONIEREN (0x1441fe0, 0x17) -> 0x594b10, RAUBUEBERFALL (0x1440bc8, 0x17) -> 0x594db0,
   REGENERATION (0x1442008, 0x17) and ZIELSCHEIBE / STROHPUPPE (0x1440acc / 0x1440ad0, 0x17) -> 0x5bba00;
   with the flag 0x400: LAGER (0x1441f90) -> 0x5b26b0 (param_1, param_1 + 0x5d), GELAGE (0x1441f88) ->
   0x5b9f30, TRANSPORT (0x1441fa0) -> 0x5af1a0; 0x5912c0 every frame */

int ai_OpenTransportBuy(); int ai_OpenTransportSell();

void ai_Unk594e50(int param_1)
{
    int r;
    int ausruesten;
    int lauer;
    int angriff;
    int schutzgeld;
    int spionieren;
    int raub;
    int regen;
    int stroh;
    int ziel;
    int lager;
    int gelage;
    int transport;

    ausruesten = 0;
    lauer = 0;
    angriff = 0;
    schutzgeld = 0;
    spionieren = 0;
    raub = 0;
    regen = 0;
    stroh = 0;
    ziel = 0;
    lager = 0;
    gelage = 0;
    transport = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk594e50, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            ausruesten = sv_RegisterNamePlate("contact_AUSRUESTEN", U_I((unsigned)&full_data_1441fb8), 0xd);
            lauer = sv_RegisterNamePlate("contact_AUF_LAUER_LEGEN", U_I((unsigned)&full_data_1441fb0), 0x15);
            angriff = sv_RegisterNamePlate("contact_ANGRIFF", U_I((unsigned)&full_data_1441fa8), 0x17);
            if (gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x35) != 0)
                stroh = sv_RegisterNamePlate("ob_STROHPUPPE", U_I((unsigned)&full_data_1440ad0), 0x17);
            if (gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x34) != 0)
                ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", U_I((unsigned)&full_data_1440acc), 0x17);
            schutzgeld = sv_RegisterNamePlate("contact_SCHUTZGELD_ERPRESSEN", U_I((unsigned)&full_data_1442010), 0x17);
            spionieren = sv_RegisterNamePlate("contact_GEBAEUDE_AUSSPIONIEREN", U_I((unsigned)&full_data_1441fe0), 0x17);
            raub = sv_RegisterNamePlate("contact_RAUBUEBERFALL", U_I((unsigned)&full_data_1440bc8), 0x17);
            regen = sv_RegisterNamePlate("contact_REGENERATION", U_I((unsigned)&full_data_1442008), 0x17);
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            gelage = sv_RegisterNamePlate("contact_GELAGE", U_I((unsigned)&full_data_1441f88), 0xe);
            transport = sv_RegisterNamePlate("TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
        }
        ai_Unk5912c0();
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(param_1 + 0x5d));
            else if (U_I((unsigned)&ai_ClickedObject) == gelage)
                ai_Unk5b9f30(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ausruesten)
                ai_Unk5bb350(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ziel || U_I((unsigned)&ai_ClickedObject) == stroh ||
                     U_I((unsigned)&ai_ClickedObject) == regen)
                ai_Unk5bba00(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == lauer)
                ai_Unk594180(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == angriff)
                ai_Unk594470(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == schutzgeld)
                ai_Unk594790(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == spionieren)
                ai_Unk594b10(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == raub)
                ai_Unk594db0(param_1);
        }
        r = sv_RunFrame(ai_Unk594e50, 0x67fff);
    }
}
