/* 0x54cc50, a Ghidra miss (disassembled; 1344 B): the run handler of the title ceremony ("SPECIAL\TITEL" for the own figure, "SPECIAL\TITEL_ANDERE" for another): the figure +0xb0 (esi) of the player (word +8, rec) gets the title +0xac (a byte, 2 = the citizenship "_VERLEIHUNG_BUERGERRECHT", 7 / 8 / 9 = the higher ranks): state 0 opens the window, the news (0x42a1 / 0x42a3 / 0x429f / 0x1617 / 0x1618 for the own figure, 0x42a0 / 0x42a2 / 0x42a4 / 0x161a / 0x161b for another: the male / female title texts "_TITEL_MAENNLICH" / "_TITEL_WEIBLICH" by +9) and the fanfare sounds 0x4c27d0 ("FanfarePositivHeldenhaftKurz"), the state 1 waits for the answer (0xf8a668 = 0, 0x6f2630 for the click of the other player) and frees the event (ROUGH) */

void he_Unk54cc50(int ev)
{
    int ebp, esi, st, w, t;

    ebp = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    if (*(unsigned short *)(unsigned)&ai_PlayerId != *(unsigned short *)(ev + 8) || *(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) == 1)
        return;
    esi = gm_FindChar(*(int *)(ev + 0xb0));
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1 || *(unsigned short *)ebp == 0xffff || esi == 0) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        if (*(int *)(ev + 0x74) == 0) {
            if (esi == ebp)
                he_Unk53d6f0(ev, 0, 0, (int)"SPECIAL\\TITEL", 0x5fd);
            else
                he_Unk53d6f0(ev, 0, 0, (int)"SPECIAL\\TITEL_ANDERE", 0x5fe);
        }
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        t = *(unsigned char *)(ev + 0xac);
        if (esi == ebp) {
            if (*(int *)(ev + 0xb4) == 0) {
                cm_BeginFieldBlock(esi, *(int *)(esi + 4));
                cm_AddFieldDelta(1, 1, 0xd, ev + 0xac);
                cm_SendFieldDeltas();
                *(int *)(ev + 0xb4) = 1;
            }
            if (t == 9)
                io_PrintText(0x42a3, ev);
            else if (t == 8)
                io_PrintText(0x42a1, ev);
            else if (t == 7)
                io_PrintText(0x429f, ev);
            else if (t == 2)
                io_PrintText(0x1618, ((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x1fc, ev, *(unsigned short *)esi, (*(char *)(esi + 9) != 0 ? 10 : 0) + 0x116);
            else
                io_PrintText(0x1617, ((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x1fc, ev, *(unsigned short *)esi, t + (*(char *)(esi + 9) == 0 ? 0x114 : 0x11e));
            snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftKurz", 0, 0);
            snd_QueueVoice(-8, 0, "_VERLEIHUNG_BUERGERRECHT", 0, 0xfa);
        } else {
            w = *(char *)(esi + 9) != 0 ? 10 : 0;
            if (t == 9)
                io_PrintText(0x42a4, *(unsigned short *)esi, w + 0x11d, *(unsigned short *)esi);
            else if (t == 8)
                io_PrintText(0x42a2, *(unsigned short *)esi, w + 0x11c, *(unsigned short *)esi);
            else if (t == 7)
                io_PrintText(0x42a0, *(unsigned short *)esi, w + 0x11b, *(unsigned short *)esi);
            else if (t == 2)
                io_PrintText(0x161b, *(unsigned short *)esi);
            else
                io_PrintText(0x161a, *(unsigned short *)esi, *(unsigned short *)esi, t + (*(char *)(esi + 9) == 0 ? 0x114 : 0x11e), *(unsigned short *)esi);
            snd_QueueVoice(-8, 0, "_TITEL_MAENNLICH", 0, 100);
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 2);
        io_PrintText(0x161c);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (st == 1) {
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8) && *(unsigned char *)(unsigned)&full_data_6f2630 == 0)
            return;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (*(unsigned char *)(unsigned)&full_data_6f2630 == 0)
            return;
        if (U_I((unsigned)&full_data_6f262c) != *(int *)(ev + 0x74))
            return;
        *(unsigned char *)(unsigned)&full_data_6f2630 = 0;
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
    }
}
