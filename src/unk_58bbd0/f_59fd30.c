/* 0x59fd30, the Rathaus contact listener (272 B): the pump 0x5309a0 (0x59fd30, 0x67fff), the flags
   0x576410 (0x200 / 0x400 / 0x800) gate "contact_GEB_KAUFEN" (0x14420f0) / "contact_TAGESORDNUNG"
   (0x1442100) / "contact_AMT_INFO" (0x143a3f0); with the gate 0x6f2448 and the key 0x1468410 byte 0x14,
   0x4f9e30 opens; the clicks open 0x59f9a0 (buy building), 0x5c1df0 (agenda) and 0x5c1b20 (office info) */

void ai_Unk59fd30(void)
{
    int kaufen = 0;
    int agenda = 0;
    int amt = 0;
    int clicked;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk59fd30, 0x67fff) != 0) {
        if (ai_Unk576410(0x200) == 1)
            kaufen = sv_RegisterNamePlate("contact_GEB_KAUFEN", U_I((unsigned)&full_data_14420f0), 0x10);
        else
            kaufen = 0;
        if (ai_Unk576410(0x400) == 1)
            agenda = sv_RegisterNamePlate("contact_TAGESORDNUNG", U_I((unsigned)&full_data_1442100), 0x16);
        else
            agenda = 0;
        if (ai_Unk576410(0x800) == 1)
            amt = sv_RegisterNamePlate("contact_AMT_INFO", U_I((unsigned)&full_data_143a3f0), 0x16);
        else
            amt = 0;
        if (U_I((unsigned)&ai_Unk6f2448) != 0 && *(unsigned char *)(unsigned)&kb_Last == 0x14)
            gm_StatisticsScreen();
        clicked = U_I((unsigned)&ai_ClickedObject);
        if (clicked != 0) {
            if (clicked == kaufen)
                ai_Unk59f9a0();
            else if (clicked == agenda)
                ai_Unk5c1df0();
            else if (clicked == amt)
                ai_Unk5c1b20();
        }
    }
}
