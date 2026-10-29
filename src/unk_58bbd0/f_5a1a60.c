/* 0x5a1a60, the guild level 1 listener (144 B): with 0x4e94a0 (the player) set, the pump 0x5309a0 (0x5a1a60,
   0x67fff) looks up "contact_ZUNFTLEVEL1" (0x143fac4, class 0x16) and opens 0x5a1000 */

void ai_Unk5a1a60(int param_1)
{
    int c = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a1a60, 0x67fff) != 0) {
        if (amt_Unk4e94a0((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0)
            c = sv_RegisterNamePlate("contact_ZUNFTLEVEL1", U_I((unsigned)&full_data_143fac4), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0 && U_I((unsigned)&ai_ClickedObject) == c)
            ai_Unk5a1000(param_1);
    }
}
