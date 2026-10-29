/* 0x5a6920, a contact listener (183 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with the mask 0x200 the
   contacts contact_LAGER (0x1441f90, 0xe), contact_AUSRUESTEN (0x1441fb8, 0xd), contact_RAZZIA (0x1442000,
   0x16), contact_GEBAEUDE_AUSSPIONIEREN (0x1441fe0, 0x17), ub_INFORMATIONSPERGAMENT (0x143a3f4, 0xc) and
   contact_TRANSPORT (0x1441fa0, 0x15); the pending click 0x6f22e4 opens 0x5b26b0 (param_1, the record 0x6f2310
   +0x14) / 0x5bb350 / 0x5a61f0 / 0x5a44e0 / 0x5a45c0 / 0x5af1a0 */

void ai_Unk5a6920(int param_1)
{
    int lager = 0;
    int ausruesten = 0;
    int razzia = 0;
    int ausspionieren = 0;
    int pergament = 0;
    int transport = 0;
    int r;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5a6920, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            ausruesten = sv_RegisterNamePlate("contact_AUSRUESTEN", U_I((unsigned)&full_data_1441fb8), 0xd);
            razzia = sv_RegisterNamePlate("contact_RAZZIA", U_I((unsigned)&full_data_1442000), 0x16);
            ausspionieren = sv_RegisterNamePlate("contact_GEBAEUDE_AUSSPIONIEREN", U_I((unsigned)&full_data_1441fe0), 0x17);
            pergament = sv_RegisterNamePlate("ub_INFORMATIONSPERGAMENT", U_I((unsigned)&full_data_143a3f4), 0xc);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0 && (U_I((unsigned)&ai_BuildingPerms) & 0x200) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == lager)
                ai_StorageWindow(param_1, U_I(U_I((unsigned)&sv_PanelShown) + 0x14));
            else if (U_I((unsigned)&ai_ClickedObject) == ausruesten)
                ai_Unk5bb350(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == razzia)
                ai_Unk5a61f0(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == ausspionieren)
                ai_Unk5a44e0((unsigned char *)param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == pergament)
                ai_Unk5a45c0(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == transport)
                ai_OpenTransport(param_1);
        }
        r = sv_RunFrame(ai_Unk5a6920, 0x67fff);
    }
}
