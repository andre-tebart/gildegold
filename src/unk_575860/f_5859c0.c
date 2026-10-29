/* 0x5859c0, the graveyard "Seance2" ritual (638 B): the first argument is unused, ev the event record (+0x48 /
   +0x4c two name ids). 0x585c40 (8, the formula ids, the buffer, 0x40d7) fills the eight formulas; the window
   "Locations_Friedhof_Seance2" (0x62a6dc) with 0x476f80 (0,0), 0x475870 (window, 1) and 0x475f70 (window, 0).
   The script "LOCATIONS_KRYPTA_SEANCE_KRYPTA_e" (0x62a6b8) is loaded (0x4287b0) and started (0x429c40) with
   the two names "%s %s" (0x60c7b8, the name +4): the player's bytes 0x770b89 / 0x770ce4 (0x4fb330, the second
   alone 0 when missing) and ev +0x4c / +0x48 (the same fallback). 0x4273c0 () == 0: the delays 0x47 and 0xf8 x8,
   each one due (the clock 0x6333cc) shows its formula text 0x515960 (the id) in section 0; the ninth entry
   closes the window (0x6f22a0 = 1); the pump 0x5309a0 (this window, 0x743c7). Else: each of the eight formulas
   becomes the news 0x4c27d0 (-7, 0, "%SEANCE_FORMEL_PART_%i_0_%i" (0x62a69c) of (id - 0x40d7) / 6 + 1 and
   (id - 0x40d7) % 6, -1, 100), then each is waited on 0x426f20 (pump 0x743c7) before and after its text
   0x515960 (the id). The window is released (0x4758f0) and the script handle returned */

void ai_Unk585c40(); int sb_IsInit(); void d2_PlaceViewWindows(int view, unsigned char flags);

int ai_Unk5859c0(char *unused, char *ev)
{
    char buf[0x400];
    char text[0x100];
    char b2[0x20];
    int ids[8];
    int a[9];
    char *name1;
    char *name2;
    int script;
    int win;
    int i;
    int h;
    unsigned int t;

    ai_Unk585c40(8, ids, b2, 0x40d7);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a6dc);
    d2_SetPointerSprite(0, 0);
    d2_PlaceViewWindows(win, 1);
    d2_SetForm(win, 0);
    script = (int)evt_Load((char *)(unsigned)&full_data_62a6b8);
    if (script != 0) {
        name1 = (char *)unk_Unk4fb330(*(unsigned char *)((unsigned)&ai_Unk770b89 + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
        if (name1 == 0)
            name1 = (char *)unk_Unk4fb330(*(unsigned char *)((unsigned)&ai_Unk770b89 + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), 0);
        name2 = (char *)unk_Unk4fb330(U_I(ev + 0x4c), U_I(ev + 0x48));
        if (name2 == 0)
            name2 = (char *)unk_Unk4fb330(U_I(ev + 0x4c), 0);
        sprintf(buf, (char *)(unsigned)"%s %s", name1 + 4, name2 + 4);
        evt_RunCommand((EVTSCRIPT *)script, buf);
    }
    if (sb_IsInit() == 0) {
        a[0] = 0x47;
        for (i = 1; i < 9; i++)
            a[i] = 0xf8;
        t = U_I((unsigned)&ai_Unk6333cc);
        i = 0;
        for (;;) {
            if (t + a[i] < (unsigned int)U_I((unsigned)&ai_Unk6333cc)) {
                t = U_I((unsigned)&ai_Unk6333cc);
                d2_SetForm(win, 0);
                if (i < 8) {
                    d2_SetForm(win, 0);
                    io_PrintText(ids[i]);
                    i++;
                } else {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk5859c0, 0x743c7) == 0)
                break;
        }
    } else {
        for (i = 0; i < 8; i++) {
            sprintf(text, (char *)(unsigned)&full_data_62a69c, (ids[i] - 0x40d7) / 6 + 1, (ids[i] - 0x40d7) % 6);
            a[i] = snd_QueueVoice(-7, 0, text, -1, 100);
        }
        for (i = 0; i < 8; i++) {
            h = a[i];
            while (sb_IsPlaying(h) == 0)
                sv_RunFrame(0, 0x743c7);
            d2_SetForm(win, 0);
            io_PrintText(ids[i]);
            while (sb_IsPlaying(h) != 0)
                sv_RunFrame(0, 0x743c7);
        }
    }
    d2_CloseView(win);
    return script;
}
