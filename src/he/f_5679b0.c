/* 0x5679b0, a Ghidra miss: the allocation handler of the tutorial event 0x83: the hint script by the profession (0x504ad0 of the kind byte 0x770ce4 of the local player: 1 / 2 / 3 "help_events_hw", 5 / 6 "help_events_alpa", 7 "help_events_kirche", 4 "help_events_wirt", 0xb "help_events_dieb") is read
   (0x567810), the first run is the first table entry 0x6fa260 (ROUGH) */

void he_Unk5679b0(int ev)
{
    int k;

    k = gm_TitleGroup(*(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
    if (k == 1 || k == 2 || k == 3)
        he_Unk567810("help_events_hw");
    else if (k == 5 || k == 6)
        he_Unk567810("help_events_alpa");
    else if (k == 7)
        he_Unk567810("help_events_kirche");
    else if (k == 4)
        he_Unk567810("help_events_wirt");
    else if (k == 0xb)
        he_Unk567810("help_events_dieb");
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x52) = U_I((unsigned)&full_data_6fa260);
    *(short *)(ev + 0x56) = *(short *)(unsigned)&full_data_6fa264;
    *(int *)(ev + 0x58) = U_I((unsigned)&full_data_6fa268);
}
