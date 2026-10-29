/* 0x5a5080, a contact listener (200 B, room 0x60 of the driver 0x59e420): 0x52e190, the pump 0x5309a0 (this,
   0x67fff); with the mask 0x200 the targets ob_ZIELSCHEIBE (0x1440acc, 0xc) / contact_NACHT (0x1440ad0, 0xc)
   and, when the mask is still set and the pending click 0x6f22e4 is one of them, 0x5bba00 (param_1) */

void ai_Unk5a5080(int param_1)
{
    int ziel = 0;
    int nacht = 0;

    sv_ReleaseNamePlates();
    if (sv_RunFrame(ai_Unk5a5080, 0x67fff) == 0)
        return;
    do {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", U_I((unsigned)&full_data_1440acc), 0xc);
            nacht = sv_RegisterNamePlate("contact_NACHT", U_I((unsigned)&full_data_1440ad0), 0xc);
        }
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) && U_I((unsigned)&ai_ClickedObject) != 0 &&
            (U_I((unsigned)&ai_ClickedObject) == ziel || U_I((unsigned)&ai_ClickedObject) == nacht))
            ai_Unk5bba00(param_1);
    } while (sv_RunFrame(ai_Unk5a5080, 0x67fff) != 0);
}
