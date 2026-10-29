/* 0x585cb0, the graveyard seance outcome (633 B): ch = the character, ev = the event record (+0x14 / +0x4c the
   voice pair). 0x4c24e0 (ev + 0x14, ev + 0x4c) the voice variant, 0x5859c0 (ch, ev) the ritual (the script handle
   kept), 0x4f41b0 (ch, 4, ids, &cnt) the ghost candidates; mode = (its result == 0), overridden to 2 when
   0x42ec20 () + 0x4f2660 (ch) - 0.1f < 1.0f. The window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 /
   0x475600, the sound 0x52ea30, 0xf8a668 = -1; 0x4c24e0 again (the voice handle). Mode 0 (a ghost found): the
   records 0x4c4ed0 (the id, the character + 4), 0x56fbc0 (ch, 1), the text 0x40ef (0x40cc, the ghost's id word)
   and the news 0x4c27d0 (the voice, 0, "GEIST_KOMMT" 0x62a70c, 0x42ec00 (8) & 0xffff, 0xfa). Mode 1:
   0x42ec00 (0x200) & 0xffff + 0x200 times 0x20 as the amount, 0x4c47e0 (the character + 4, -1, 0x6f1bc5, the
   amount), the text 0x40f0 (0x40cc, the amount) and the same news. Mode 2: the text 0x40d3 (0x40cc) and the
   news "GEIST_KOMMT_NICHT" (0x62a6f8). The pump loop 0x5309a0 (this window, 0x777c7): the escape 0x14673a0 ->
   0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the script freed (0x429e90) */

int snd_VoiceVariant(); char *gv_FindAccused(char *c, int max, int *ids, int *cnt); float unk_Unk4f2660(); float util_RandomF();
void cm_SendProofExecuted(int a, int b); 

void ai_Unk585cb0(char *ch, char *ev)
{
    int ids[4];
    int cnt;
    char *p;
    float f;
    int win;
    int voice;
    int mode;
    int script;
    int i;
    int n;

    snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    script = ai_Unk5859c0(ch, ev);
    p = (char *)gv_FindAccused((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 4, ids, &cnt);
    mode = p == 0;
    f = unk_Unk4f2660(ch);
    if (util_RandomF() + f - (0.100000001f) < *(float *)(unsigned)&ai_Unk5eb780)
        mode = 2;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_SetForm(win, 1);
    voice = snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    if (mode == 0) {
        for (i = 0; i < cnt; i++)
            cm_SendProofExecuted(ids[i], *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
        he_Unk56fbc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 1);
        io_PrintText(0x40ef, 0x40cc, *(unsigned short *)p);
        i = util_Random(8) & 0xffff;
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, i, 0xfa);
    } else if (mode == 1) {
        n = ((util_Random(0x200) & 0xffff) + 0x200) << 5;
        cm_SendMoveGoods(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
        io_PrintText(0x40f0, 0x40cc, n);
        i = util_Random(8) & 0xffff;
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, i, 0xfa);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        i = util_Random(8) & 0xffff;
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a6f8, i, 0xfa);
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk585cb0, 0x777c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
