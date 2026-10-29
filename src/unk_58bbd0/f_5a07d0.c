/* 0x5a07d0, the law book / application listener of the town hall (384 B): the pump 0x5309a0 (0x5a07d0,
   0x67fff); the flags 0x576410 (0x1000: the three "ob_GESETZBUCH_*" 0x143f190 / 0x143f194 / 0x143f18c,
   0x8000: "contact_BEWERBEN" 0x14420e0, 0x2000 with the player's flag +0x1c8 & 0x4000 and the byte
   0x770b8d == 1: "contact_BUERGERRECHT" 0x14420e8); the clicks open 0x5c2160 (0x112 / 0x111 / 0x113), the
   citizen right 0x59fe40 (param_1) or the application 0x5a0590 */

void ai_Unk5a07d0(int param_1)
{
    int steuern = 0;
    int strafrecht = 0;
    int verfassung = 0;
    int bewerben = 0;
    int recht = 0;
    int c;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5a07d0, 0x67fff) != 0) {
        if (ai_Unk576410(0x1000) == 1) {
            steuern = sv_RegisterNamePlate("ob_GESETZBUCH_STEUERN", U_I((unsigned)&full_data_143f190), 0xc);
            strafrecht = sv_RegisterNamePlate("ob_GESETZBUCH_STRAFRECHT", U_I((unsigned)&full_data_143f194), 0xc);
            verfassung = sv_RegisterNamePlate("ob_GESETZBUCH_VERFASSUNG", U_I((unsigned)&full_data_143f18c), 0xc);
        } else {
            verfassung = 0;
            strafrecht = 0;
            steuern = 0;
        }
        if (ai_Unk576410(0x8000) == 1)
            bewerben = sv_RegisterNamePlate("contact_BEWERBEN", U_I((unsigned)&full_data_14420e0), 0x16);
        else
            bewerben = 0;
        if ((U_I(((unsigned)&ai_Selected + 0x40) + U_W((unsigned)&ai_PlayerId) * 0x21c) & 0x4000) == 0 ||
            *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 1 ||
            ai_Unk576410(0x2000) != 1)
            recht = 0;
        else
            recht = sv_RegisterNamePlate("contact_BUERGERRECHT", U_I((unsigned)&full_data_14420e8), 0x16);
        c = U_I((unsigned)&ai_ClickedObject);
        if (c != 0) {
            if (c == steuern)
                ai_Unk5c2160(0x112);
            else if (c == strafrecht)
                ai_Unk5c2160(0x111);
            else if (c == verfassung)
                ai_Unk5c2160(0x113);
            else if (c == recht)
                ai_Unk59fe40(param_1);
            else if (c == bewerben)
                ai_Unk5a0590();
        }
    }
}
