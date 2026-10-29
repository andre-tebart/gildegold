/* 0x5a2de0, the confession listener (128 B): the pump 0x5309a0 (this, 0x67fff) looks up "contact_BEICHTE"
   (0x14407b8, class 0x16) while the flag 0x6f231c bit 0x200 is clear and opens 0x5a2b60 on its id */

void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate();

void ai_Unk5a2de0(int param_1)
{
    int c = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a2de0, 0x67fff) != 0) {
        if ((U_I((unsigned)&ai_BuildingPerms) & 0x200) == 0)
            c = sv_RegisterNamePlate("contact_BEICHTE", U_I((unsigned)&full_data_14407b8), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0 && (U_I((unsigned)&ai_BuildingPerms) & 0x200) == 0 && U_I((unsigned)&ai_ClickedObject) == c)
            ai_Unk5a2b60(param_1);
    }
}
