/* 0x585f30, the graveyard seance with a sacrifice (717 B): ch / ev as 0x585cb0. 0x4c24e0 (ev + 0x14, ev + 0x4c)
   the voice, 0x5859c0 (ch, ev) the ritual (the script handle); the chance 0x42ec20 () + 0x4f2660 (ch) - 0.4f <
   1.0f -> no ghost (mode 2). Else the sacrifice: 0x50a640 (the player) * 0.25f clamped to 160000 .. 800000
   (0x27100 / 0xc3500); 0x4c47e0 (the character + 4, -1, 0x6f1bc5, the amount); the message (type 0x8e at +4,
   the character 0x770b84 at +8, ch + 1 at +0xc, the clock 0x770ae0 .. 0x770aec at +0x28, 1 at +0x36, the
   amount at +0x58) through 0x4c4b70; 0x56fbc0 (the character, 3). The window "Locations_Friedhof_Friedhof"
   (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, 0xf8a668 = -1; the news 0x4c27d0 (the voice, 0,
   "GEIST_KOMMT" 0x62a70c / "GEIST_KOMMT_NICHT" 0x62a6f8, 0x42ec00 (8) & 0xffff, 0xfa) and the text 0x40f1
   (0x40cc, the amount) / 0x40d3 (0x40cc); the pump loop 0x5309a0 (this window, 0x777c7): the escape -> 0x6f22a0
   = 1; the pump 0 -> 0x4758f0 (window), the script freed (0x429e90) */

void ai_Unk585f30(char *ch, char *ev)
{
    char msg[0xf8];
    int voice;
    int script;
    int win;
    int amount;
    int mode;
    int i;

    voice = snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    script = ai_Unk5859c0(ch, ev);
    amount = 0;
    mode = 0;
    if (util_RandomF() + unk_Unk4f2660(ch) - (0.400000006f) < *(float *)(unsigned)&ai_Unk5eb780) {
        mode = 2;
    } else {
        amount = (int)(gm_GetWorth(*(unsigned short *)(unsigned)&ai_PlayerId) * 0.25f);
        if (amount > 0x27100) {
            if (amount >= 0xc3500)
                amount = 0xc3500;
        } else {
            amount = 0x27100;
        }
        cm_SendMoveGoods(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, amount);
        memset(msg, 0, 0xf8);
        *(unsigned char *)(msg + 0x04) = 0x8e;
        *(unsigned char *)(msg + 0x36) = 1;
        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        *(int *)(msg + 0x0c) = U_I(ch + 1);
        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
        *(int *)(msg + 0x58) = amount;
        cm_SendLongMessage(msg);
        he_Unk56fbc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 3);
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_SetForm(win, 1);
    i = util_Random(8) & 0xffff;
    if (mode == 0) {
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, i, 0xfa);
        io_PrintText(0x40f1, 0x40cc, amount);
    } else {
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a6f8, i, 0xfa);
        io_PrintText(0x40d3, 0x40cc);
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk585f30, 0x777c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
