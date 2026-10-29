/* 0x5a1960, the guild level 3 listener (256 B): the text id by the office byte 0x770ce9 + slot * 0x21c (0x1e
   0x167c, 0x1f / 0x21 0x166a, 0x20 0x1674); the pump 0x5309a0 (0x5a18d0, 0x67fff) looks up "ob_ZUNFTKASSE"
   (0x1440a24, class 0x16) and, with 0x4e95a0 (the player) and the id, "contact_ZUNFTLEVEL3" (the text of the
   id from 0x143a1c0); the clicks open 0x5a0950 (param_1) or 0x5a17b0 */

void ai_Unk5a1960(int param_1)
{
    int kasse;
    int level3 = 0;
    int id = 0;
    int r;
    char c;

    c = *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + U_W((unsigned)&ai_PlayerId) * 0x21c);
    if (c == 0x1e)
        id = 0x167c;
    else if (c == 0x1f || c == 0x21)
        id = 0x166a;
    else if (c == 0x20)
        id = 0x1674;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a18d0, 0x67fff) != 0) {
        kasse = sv_RegisterNamePlate("ob_ZUNFTKASSE", U_I((unsigned)&full_data_1440a24), 0x16);
        r = amt_CanUseMayorPower3((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
        if (r != 0 && id != 0)
            level3 = sv_RegisterNamePlate("contact_ZUNFTLEVEL3", (char *)U_I((unsigned)&d2_Strings + id * 4), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == kasse)
                ai_Unk5a0950(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == level3)
                ai_Unk5a17b0();
        }
    }
}
