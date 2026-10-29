/* 0x586200, the graveyard seance with a ghost lottery (860 B): ch / ev as 0x585cb0. 0x4c24e0 (ev + 0x14, ev +
   0x4c) the voice, 0x5859c0 (ch, ev) the ritual (the script handle). The fifteen wanted items 0x2b8, 0x2bb,
   0x2bc, 0x2bf, 0x2c2, 0x2c3, 0x2bd, 0x2c1, 0x2c4, 0x2c5, 0x2c6, 0x2ba, 0x2c0, 0x2be, 0x2b9 are looked up
   in the character's inventory (0x4a5760 (ch, 0xf, rec, st)); (st[0] == 6 and st[2] == 0) or st[1] == 0xf ->
   mode 1; else a random free record (0x42ec00 (0xf)) gives mode 0 and its item. The chance 0x42ec20 () +
   0x4f2660 (ch) - 0.2f < 1.0f -> mode 2 (no ghost). The window "Locations_Friedhof_Friedhof" (0x62a5bc) with
   0x476f80 / 0x475600, the sound 0x52ea30, 0xf8a668 = -1; mode 0: 0x4c4880 (the character + 4, -1, the item,
   1, 0x6f1bc5, 0), 0x56fbc0 (the character, 2), the text 0x40f2 (0x40cc, the item * 2 + 0x9a8) and the news
   0x4c2760 (the voice, 0, "GEIST_KOMMT" 0x62a70c, 0x42ec00 (8) & 0xffff); mode 1: 0x42ec00 (0x400) & 0xffff +
   1000 times 0x20, 0x4c47e0 (the character + 4, -1, 0x6f1bc5, the amount), the text 0x40f3 and the same news;
   mode 2: the text 0x40d3 (0x40cc) and "GEIST_KOMMT_NICHT" (0x62a6f8). The pump loop 0x5309a0 (this window,
   0x777c7): the escape -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the script freed (0x429e90) */

void ai_Unk4a5760(struct G5CH *p, int n, struct G9REC *rec, int *st);

void ai_Unk586200(char *ch, char *ev)
{
    int rec[15][4];
    int st[3];
    int win;
    int voice;
    int script;
    int item;
    int mode;
    int i;
    int n;

    voice = snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    script = ai_Unk5859c0(ch, ev);
    memset(rec, 0, sizeof rec);
    rec[0][0] = 0x2b8;
    rec[1][0] = 0x2bb;
    rec[2][0] = 0x2bc;
    rec[3][0] = 0x2bf;
    rec[4][0] = 0x2c2;
    rec[5][0] = 0x2c3;
    rec[6][0] = 0x2bd;
    rec[7][0] = 0x2c1;
    rec[8][0] = 0x2c4;
    rec[9][0] = 0x2c5;
    rec[10][0] = 0x2c6;
    rec[11][0] = 0x2ba;
    rec[12][0] = 0x2c0;
    rec[13][0] = 0x2be;
    rec[14][0] = 0x2b9;
    ai_Unk4a5760((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0xf, (int *)rec, st);
    item = 0;
    if (st[0] == 6 && st[2] == 0) {
        mode = 1;
    } else if (st[1] == 0xf) {
        mode = 1;
    } else {
        i = util_Random(0xf) & 0xffff;
        while (rec[i][1] != 0)
            i = (i + 1) % 0xf;
        mode = 0;
        item = rec[i][0];
    }
    if (util_RandomF() + unk_Unk4f2660(ch) - 0.2f < *(float *)(unsigned)&ai_Unk5eb780)
        mode = 2;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_SetForm(win, 1);
    if (mode == 0) {
        cm_SendExSellObject(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, item, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        he_Unk56fbc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 2);
        io_PrintText(0x40f2, 0x40cc, item * 2 + 0x9a8);
        snd_PlayVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, util_Random(8) & 0xffff);
    } else if (mode == 1) {
        n = ((util_Random(0x400) & 0xffff) + 0x3e8) << 5;
        cm_SendMoveGoods(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
        io_PrintText(0x40f3, 0x40cc, n);
        snd_PlayVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, util_Random(8) & 0xffff);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        snd_PlayVoice(voice, 0, (char *)(unsigned)&full_data_62a6f8, util_Random(8) & 0xffff);
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk586200, 0x777c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
