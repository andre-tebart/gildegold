/* 0x5a4f70, a contact listener (260 B, room 0x5c of the driver 0x59e420): 0x52e190, the pump 0x5309a0 (this,
   0x67fff); with the mask 0x200 the contacts contact_AUSRUESTEN (0x1441fb8, 0xd) / contact_REGENERATION
   (0x1442008, 0x17), with 0x400 contact_LAGER (0x1441f90, 0xe) / contact_TRANSPORT (0x1441fa0, 0x15); the
   pending click 0x6f22e4 opens 0x5b26b0 (param_1, the record 0x6f2310 field +0x14) for LAGER, 0x5af1a0 for
   TRANSPORT, 0x5bb350 for AUSRUESTEN and 0x5bba00 for REGENERATION */

void ai_Unk5a4f70(int param_1)
{
    int ausruesten = 0;
    int regeneration = 0;
    int lager = 0;
    int transport = 0;
    int click;

    sv_ReleaseNamePlates();
    if (sv_RunFrame(ai_Unk5a4f70, 0x67fff) == 0)
        return;
    do {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            ausruesten = sv_RegisterNamePlate("contact_AUSRUESTEN", U_I((unsigned)&full_data_1441fb8), 0xd);
            regeneration = sv_RegisterNamePlate("contact_REGENERATION", U_I((unsigned)&full_data_1442008), 0x17);
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
        }
        click = U_I((unsigned)&ai_ClickedObject);
        if (click != 0) {
            if (click == lager)
                ai_StorageWindow(param_1, U_I(U_I((unsigned)&sv_PanelShown) + 0x14));
            else if (click == transport)
                ai_OpenTransport(param_1);
            else if (click == ausruesten)
                ai_Unk5bb350(param_1);
            else if (click == regeneration)
                ai_Unk5bba00(param_1);
        }
    } while (sv_RunFrame(ai_Unk5a4f70, 0x67fff) != 0);
}
