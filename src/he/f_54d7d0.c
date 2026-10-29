/* 0x54d7d0, a Ghidra miss (disassembled; 582 B): the run handler of the level-up message of the own player ("SPECIAL\TALENT_STUFE": the news 0x16c8, 0x16c9 + 2 * +0xb4 + the talent +0xac, the fanfare 0x4c27d0, "_TALENT_MAIN" / "_TALENT_STUFE" (+0xb0 - 1) / "_TALENT_MAX" by the level +0xb0 (0x16e6 + level, 0x16ec + talent), 0x16f1); state 1 waits for the answer and frees the event (ROUGH) */

void he_Unk54d7d0(int ev)
{
    int rec, st, h;

    rec = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    if (*(unsigned short *)(unsigned)&ai_PlayerId != *(unsigned short *)(ev + 8) || *(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) == 1)
        return;
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1 || *(unsigned short *)rec == 0xffff) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        if (*(int *)(ev + 0x74) == 0)
            he_Unk53d6f0(ev, 0, 0, (int)"SPECIAL\\TALENT_STUFE", 0x5ec);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        io_PrintText(0x16c8);
        io_PrintText(*(int *)(ev + 0xb4) + *(unsigned char *)(ev + 0xac) * 2 + 0x16c9);
        snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftKurz", 0, 100);
        snd_QueueVoice(-8, 0, "_TALENT_MAIN", *(int *)(ev + 0xb4) + *(unsigned char *)(ev + 0xac) * 2, 0xfa);
        if (*(int *)(ev + 0xb0) < 6) {
            io_PrintText(*(int *)(ev + 0xb0) + 0x16e6);
            snd_QueueVoice(-8, 0, "_TALENT_STUFE", *(int *)(ev + 0xb0) - 1, 100);
        } else {
            io_PrintText(*(unsigned char *)(ev + 0xac) + 0x16ec);
            snd_QueueVoice(-8, 0, "_TALENT_MAX", *(unsigned char *)(ev + 0xac), 100);
        }
        snd_QueueVoice(-7, 0, "", -1, 0);
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 2);
        io_PrintText(0x16f1);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (st == 1) {
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        h = U_I((unsigned)&d2_ClickedId);
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8) && *(unsigned char *)(unsigned)&full_data_6f2630 == 0 && h == -1)
            return;
        if (h == 0) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(unsigned char *)(unsigned)&full_data_6f2630 == 0 || U_I((unsigned)&full_data_6f262c) != *(int *)(ev + 0x74))
            return;
        *(unsigned char *)(unsigned)&full_data_6f2630 = 0;
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
    }
}
