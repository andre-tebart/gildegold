/* 0x5a6a90, a contact listener (172 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with the mask 0x200 the
   targets ob_ZIELSCHEIBE (0x1440acc, 0xc), contact_NACHT (0x1440ad0, 0xc) and contact_REGENERATION
   (0x1442008, 0x17) and, when the mask is still set and the pending click 0x6f22e4 is one of them, 0x5bba00
   (param_1) */

void ai_Unk5a6a90(int param_1)
{
    int ziel;
    int nacht;
    int regeneration;
    int r;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5a6a90, 0x67fff);
    while (r != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", U_I((unsigned)&full_data_1440acc), 0xc);
            nacht = sv_RegisterNamePlate("contact_NACHT", U_I((unsigned)&full_data_1440ad0), 0xc);
            regeneration = sv_RegisterNamePlate("contact_REGENERATION", U_I((unsigned)&full_data_1442008), 0x17);
            if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) && U_I((unsigned)&ai_ClickedObject) != 0 &&
                (U_I((unsigned)&ai_ClickedObject) == ziel || U_I((unsigned)&ai_ClickedObject) == nacht || U_I((unsigned)&ai_ClickedObject) == regeneration))
                ai_Unk5bba00(param_1);
        }
        r = sv_RunFrame(ai_Unk5a6a90, 0x67fff);
    }
}
