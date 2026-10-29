/* 0x5a18d0, the guild level 2 listener (144 B): with 0x4e94e0 (the player) set, the pump 0x5309a0 (0x5a18d0,
   0x67fff) looks up "contact_ZUNFTLEVEL2" (0x143fb34, class 0x16) and opens 0x5a0f10 */

void ai_Unk5a18d0(int param_1)
{
    int c = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a18d0, 0x67fff) != 0) {
        if (amt_CanUseMayorPower2((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0)
            c = sv_RegisterNamePlate("contact_ZUNFTLEVEL2", U_I((unsigned)&full_data_143fb34), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0 && U_I((unsigned)&ai_ClickedObject) == c)
            ai_Unk5a0f10(param_1);
    }
}
