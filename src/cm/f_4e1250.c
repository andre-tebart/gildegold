/* 0x4e1250, a death (the message m: +0x34 the dying person): the scene "Tod.ed3" with "cutscenes\\tod\\enter_TOD.esc", the bells "GLOCKEN_r", the
   voices "_TOD_HS" 0 .. 6 with the pauses 0xbb8 / 0xfa0 / 0x7d0 / 0x1f4, a fade out over 0x339 ticks (0x44a170); then by the flags +0x1cc
   (1 = heir / 2) of the person and whether 0x506480 finds a successor the text 0x40fb / 0x40fc / 0x1ab3 / 0x1ab4 (0x4e5470) and the
   field blocks 0x1c / 0x24 are sent (0x4c3c00), or the death is reported to the player's owner (0x5c4150). ROUGH */

int ai_Unk4e1250(char *m)
{
    char buf[0x400];
    char big[0x1c00];
    char *a;
    int ages[6] = {8, 10, 12, 15, 18, 20};
    int stage, i, sc, snd, t0, rc, ok;
    float f, v[4];

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    stage = 6;
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    for (i = 0; i < 6; i++) {
        if ((unsigned short)ai_TimeHour < ages[i]) {
            stage = i;
            break;
        }
    }
    ai_Unk4e3620("cd2\\AmKuehlenGrabe.mp3");
    snd = snd_LoadVoiceBank("tod.sbf", 1);
    ai_Unk4e2e60("Tod.ed3");
    ai_Unk4e32f0("Sky_Schoen_02");
    d3_SetLightSet(stage, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 0, 0);
    sc = ai_Unk4e2c30("cutscenes\\tod\\enter_TOD.esc", &ch_EmptyName);
    rc = ai_Unk4e3490(0xbb8, 0, 0);
    if (rc == 0) {
        for (i = 0; i < 7; i++)
            ai_Unk4e32d0(-8, "_TOD_HS", i, i == 0 ? 0 : i == 1 ? 0 : i == 2 ? 0xfa0 : i == 3 ? 0xbb8 : i == 4 ? 0x7d0 : i == 5 ? 0xfa0 : 0x1f4);
        rc = ai_Unk4e3490(0x2af8, 0, 0);
        if (rc == 0) {
            t0 = d3_Time;
            while (d3_Time < t0 + 0x339 && rc == 0) {
                if (sv_RunFrame(0, ai_Unk60d360) != 0 && dm_L.rup != 0)
                    rc = 1;
                f = (float)(d3_Time - t0) * 0.0012121212f;
                if (!(f < 1.0f))
                    f = 1.0f;
                d3_SetFogSet(0, 1, f);
            }
            d3_SetFogSet(0, 1, 0x3f800000);
            ai_Unk4e3490(0x2cec, 0, 0);
            rc = 0;
        }
    }
    sb_PlaySampleI(0, "GLOCKEN_r", 1, 0x3f, 0x7f, 0, 0);
    d2_CutsceneActive = 0;
    ok = gm_Unk506480(a, v);
    if (ok != 0) {
        if ((*(unsigned int *)(a + 0x1cc) & 1) != 0 && (*(unsigned int *)(a + 0x1cc) & 2) == 0) {
            txt_Format(buf, 0x40fb, 0x40f5, *(unsigned short *)a);
            if (ai_Unk4e5470(buf, 0x10) == 0)
                ai_Unk5c4150(ai_CharInts[ai_PlayerId][0], 0xa, ai_Unk1440c88, ai_Unk60d360);
            else {
                v[2] = 100.0f;
                cm_BeginFieldBlock(a, *(int *)(a + 4));
                cm_AddFieldDelta(4, 1, 0x1c, &v[1]);
                cm_AddFieldDelta(4, 1, 0x24, &v[2]);
                cm_SendFieldDeltas();
                cm_SendSetField(*(int *)(a + 4), 0x1cc, 4, 2, 0);
            }
        } else
            ai_Unk5c4150(ai_CharInts[ai_PlayerId][0], 0xa, ai_Unk1440c88, ai_Unk60d360);
    } else {
        if ((*(unsigned int *)(a + 0x1cc) & 1) != 0 && (*(unsigned int *)(a + 0x1cc) & 2) == 0) {
            txt_Format(buf, 0x40fc, 0x40f5, *(unsigned short *)a);
            ai_Unk4e5470(buf, 0x10);
            cm_BeginFieldBlock(a, *(int *)(a + 4));
            cm_AddFieldDelta(4, 1, 0x1c, &v[1]);
            cm_AddFieldDelta(4, 1, 0x24, &v[2]);
            cm_SendFieldDeltas();
            cm_SendSetField(*(int *)(a + 4), 0x1cc, 4, 2, 0);
        } else if ((ai_GameFlags & 8) != 0) {
            a[8] = 0;
            ai_GameOver = 1;
            if (ok != 0)
                txt_Format(buf, 0x1ab4);
            else
                txt_Format(buf, 0x1ab3, *(unsigned short *)a);
            ai_Unk4e5470(buf, 0);
            sv_PopTo((unsigned)&main_GameLoop);
        } else {
            gm_Unk506e70(a, big, 1);
            ai_Unk4e5470(big, 0x10);
        }
    }
    ai_Unk4e3060();
    ai_Unk4e2d90(sc);
    ai_Unk4e2c70(sc);
    ai_Unk4e3390();
    ai_Unk4e30c0();
    snd_ClearQueue();
    sb_UnloadSampleBank(snd);
    ai_Unk4e3640();
    ai_Unk6f0da4 = 0;
    return 0;
}
