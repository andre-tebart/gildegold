/* 0x5a3df0, the bribe listener (245 B): the pump 0x5309a0 (this, the flag byte 0x770d31 + slot * 0x21c ?
   0x67fff : 0x677ff) looks up "contact_BESTECHEN" (0x1440880, class 0x10) only with the flag set and opens the
   bribe window 0x5a3b90 for a known owner */

void ai_Unk5a3df0(int param_1)
{
    int bribe = 0;
    int mask;

    sv_ReleaseNamePlates();
    mask = *(char *)(((unsigned)&ai_Selected + 0x29) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0 ? 0x67fff : 0x677ff;
    while (sv_RunFrame(ai_Unk5a3df0, mask) != 0) {
        if (*(char *)(((unsigned)&ai_Selected + 0x29) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0)
            bribe = sv_RegisterNamePlate("contact_BESTECHEN", U_I((unsigned)&full_data_1440880), 0x10);
        if (*(char *)(((unsigned)&ai_Selected + 0x29) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0 && U_I((unsigned)&ai_ClickedObject) != 0 &&
            U_W(param_1 + 0x27) != 0xffff && U_I((unsigned)&ai_ClickedObject) == bribe)
            ai_Unk5a3b90(param_1);
        mask = *(char *)(((unsigned)&ai_Selected + 0x29) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0 ? 0x67fff : 0x677ff;
    }
}
