/* 0x566370, the run handler of the night watchman (a Ghidra miss, 1216 bytes with the jump table 0x566814): freed when the day changed (+0x44 != the day 0x770ae0); the season 0x4fe350 indexes the float tables 0x617830 (morning
   hour 8 / 7 / 8 / 9) and 0x617840 (evening hour 20 / 21 / 20 / 19); state 0 waits for the morning hour (hour in [h, h + 1)), then plays the bell "_NACHTWAECHTER_GLOCKEN_SFX_SHORT" (0x6f25ac: sound on) and with 0x6f3619
   the announcements (random(10) > 7: "_NACHTWAECHTER_ANKUENDIGUNGEN", the morning time text, "_NACHTWAECHTER_MORGENS_SPRUECHE", 0x4c27d0 (player, 0, text, -1, volume)) and sets the next run to the evening hour; state 1
   the same for the evening hour ("ABENDS_ZEIT_7 / _8 / _8_HERBST / _9", "ABENDS_SPRUECHE"), next run 22:30; state 2 at hour 22 and minute 30 .. 49 plays the bell and "NACHTS_ENDE", then frees (ROUGH) */

void he_Unk566370(int ev)
{
    int season;
    float h, m;
    int id;

    if (U_I((unsigned)&ai_Time) != *(int *)(ev + 0x44)) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    season = gm_ClockSeason((unsigned)&ai_Time);
    id = *(int *)(ev + 0xac);
    switch (*(int *)(ev + 0x70)) {
    case 0:
        h = (float)(int)(U_I((unsigned)&ai_TimeHour) & 0xffff);
        m = ((float *)(unsigned)&ai_Unk617830)[season];
        if (h < m || !(h < m + 1.0f)) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            goto lagain;
        }
        if (*(char *)(unsigned)&ai_GameState != 0)
            snd_QueueVoice(-7, 0, "_NACHTWAECHTER_GLOCKEN_SFX_SHORT", -1, 0xfa);
        if (*(char *)((unsigned)&ai_GameSwitches + 0x1) != 0 && *(char *)(unsigned)&ai_GameState != 0 && (unsigned short)util_Random(10) > 7 && *(char *)(unsigned)&ai_GameState != 0) {
            snd_QueueVoice(id, 0, "_NACHTWAECHTER_ANKUENDIGUNGEN", -1, 0xfa);
            if (m == 7.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_7", -1, 0xfa);
            } else if (m == 8.0f) {
                if (season == 2)
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_HERBST", -1, 0xfa);
                else
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_8", -1, 0xfa);
            } else if (m == 9.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_9", -1, 0x3e8);
            }
            if ((unsigned short)util_Random(10) > 7)
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_SPRUECHE", -1, 0x3e8);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(short *)(ev + 0x56) = (short)(int)((float *)(unsigned)&ai_Unk617840)[season];
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        h = (float)(int)(U_I((unsigned)&ai_TimeHour) & 0xffff);
        m = ((float *)(unsigned)&ai_Unk617840)[season];
        if (h < m || !(h < m + 1.0f)) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            goto lagain;
        }
        if (*(char *)(unsigned)&ai_GameState != 0)
            snd_QueueVoice(-7, 0, "_NACHTWAECHTER_GLOCKEN_SFX_SHORT", -1, 0xfa);
        if (*(char *)((unsigned)&ai_GameSwitches + 0x1) != 0 && *(char *)(unsigned)&ai_GameState != 0 && (unsigned short)util_Random(10) > 7 && *(char *)(unsigned)&ai_GameState != 0) {
            snd_QueueVoice(id, 0, "_NACHTWAECHTER_ANKUENDIGUNGEN", -1, 0xfa);
            if (m == 19.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_7", -1, 0xfa);
            } else if (m == 20.0f) {
                if (season == 2)
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_8_HERBST", -1, 0xfa);
                else
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_8", -1, 0xfa);
            } else if (m == 21.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_9", -1, 0xfa);
            }
            if ((unsigned short)util_Random(10) > 7)
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_SPRUECHE", -1, 0x3e8);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(short *)(ev + 0x56) = 0x16;
        *(int *)(ev + 0x58) = 0x1e;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (*(unsigned short *)(unsigned)&ai_TimeHour < 0x16 || (int)U_I(((unsigned)&ai_Time + 0x6)) < 0x1e || (int)U_I(((unsigned)&ai_Time + 0x6)) >= 0x32) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            goto lagain;
        }
        if (*(char *)(unsigned)&ai_GameState != 0) {
            snd_QueueVoice(-7, 0, "_NACHTWAECHTER_GLOCKEN_SFX_SHORT", -1, 0xfa);
            if (*(char *)((unsigned)&ai_GameSwitches + 0x1) != 0 && (unsigned short)util_Random(10) > 7)
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_NACHTS_ENDE", -1, 0xfa);
        }
        he_Unk53e1f0((int *)ev);
        return;
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
lagain:
    gm_AddTime((unsigned)&ai_Time, 0, 0, 1);
}
