/* 0x552ca0, the run handler of the award ceremony of an imperial office ("_VERLEIHUNG_REICHSAMT_.." texts; the figure +8 is the player's, +0xbc the awarded person, +0xc0 the office 0x23 .. 0x25, +0xac the clock of the award; ROUGH): nothing unless the figure is the player's (0x6f25b8) and the game runs; state 0 opens the window "SPECIAL\TITEL" (the player is awarded) or "SPECIAL\TITEL_ANDERE", plays the fanfare "FanfarePositivHeldenhaftLang" and the spoken address / office texts (0x4c27d0 (-7, 0, name, 0, 100) with the text names by sex (person +9) and rank (person +0xd: 9 = _02, 8 = _01, else _00)), prints the news (0x515960) and the click sound; state 1 waits for the answer (0xf8a668 == 0 or the confirm 0x6f2630) and closes; a gone figure / person frees the record */

void he_Unk552ca0(int ev)
{
    int id = *(unsigned short *)(ev + 8);
    int ch = (unsigned)&ai_Chars + id * 0x21c;
    int p, st, own, base, tid, w, off;
    char *s1;
    char *s2;
    char cl;

    if (*(unsigned short *)(unsigned)&ai_PlayerId != id || *(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) == 1)
        return;
    p = gm_FindChar(*(int *)(ev + 0xbc));
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1 || *(unsigned short *)ch == 0xffff || p == 0) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        if (*(int *)(ev + 0x74) == 0) {
            if (p == ch)
                he_Unk53d6f0(ev, 0, 0, (int)"SPECIAL\TITEL", 0x5fd);
            else
                he_Unk53d6f0(ev, 0, 0, (int)"SPECIAL\TITEL_ANDERE", 0x5fe);
        }
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        w = *(unsigned char *)(p + 9) == 1;
        off = *(unsigned char *)(p + 9) == 0 ? 0x253 : *(int *)(ev + 0xc0) + 0x27b;
        if (p == ch) {
            int rank = *(unsigned char *)(p + 0xd);

            if (rank == 9) {
                base = 0x42a9;
                s1 = w ? "_VERLEIHUNG_REICHSAMT_ANREDE_HS_W_02" : "_VERLEIHUNG_REICHSAMT_ANREDE_HS_M_02";
            } else if (rank == 8) {
                base = 0x42a7;
                s1 = w ? "_VERLEIHUNG_REICHSAMT_ANREDE_HS_W_01" : "_VERLEIHUNG_REICHSAMT_ANREDE_HS_M_01";
            } else {
                base = 0x42a5;
                s1 = w ? "_VERLEIHUNG_REICHSAMT_ANREDE_HS_W_00" : "_VERLEIHUNG_REICHSAMT_ANREDE_HS_M_00";
            }
            tid = 0;
            if (w)
                base++;
            snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftLang", 0, 0x64);
            snd_QueueVoice(-7, 0, s1, 0, 0x64);
            s2 = 0;
            if (*(int *)(ev + 0xc0) == 0x25) {
                tid = 0x42ab;
                s2 = w ? "_VERLEIHUNG_REICHSAMT_WEIBLICH_HS_00" : "_VERLEIHUNG_REICHSAMT_MAENNLICH_HS_00";
            } else if (*(int *)(ev + 0xc0) == 0x24) {
                tid = 0x42af;
                s2 = w ? "_VERLEIHUNG_REICHSAMT_WEIBLICH_HS_02" : "_VERLEIHUNG_REICHSAMT_MAENNLICH_HS_02";
            } else if (*(int *)(ev + 0xc0) == 0x23) {
                tid = 0x42ad;
                s2 = w ? "_VERLEIHUNG_REICHSAMT_WEIBLICH_HS_01" : "_VERLEIHUNG_REICHSAMT_MAENNLICH_HS_01";
            }
            snd_QueueVoice(-7, 0, s2, 0, 0x64);
            snd_QueueVoice(-7, 0, "MausclickLinks_s_01", -1, 0);
            io_PrintText(tid, base, ev + 0xac, off);
        } else {
            tid = 0;
            if (*(int *)(ev + 0xc0) == 0x25)
                tid = 0x42ac;
            else if (*(int *)(ev + 0xc0) == 0x24)
                tid = 0x42b0;
            else if (*(int *)(ev + 0xc0) == 0x23)
                tid = 0x42ae;
            io_PrintText(tid, *(unsigned short *)p, off);
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
        cl = *(char *)(unsigned)&full_data_6f2630;
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8) && cl == 0)
            return;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (cl == 0 || U_I((unsigned)&full_data_6f262c) != *(int *)(ev + 0x74))
            return;
        *(char *)(unsigned)&full_data_6f2630 = 0;
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
    }
}
