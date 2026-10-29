/* 0x4ded10, "cut_MainDuell": the duel of the characters m + 0x34 / m + 0x38 (m + 0x7c the scene; with 0x6f0d8c set only the result is
   announced, 0x4debe0): the scene "cutscenes\\duell\\enter_DUELL.esc" with music, the two duellists (copies of the character records, put
   at "dummy_DUELLANT_A_ENTRY" / "_B_ENTRY"), the pistols "ob_PISTOLE", then three rounds (0x4de900 for both sides: the intro texts, the
   shot 0x4de640) until 0x6f0d94 (a fighter down), the result texts 0x111c .. 0x1123 and the damage dealt (0x6f0da8 / 0x6f0da9) taken off
   the life with 0x4fca40, the doctor ("_DUELL_INTRO_C2_DOKTOR"), and the message to the other characters. ROUGH: control flow of
   the shot rounds and the text numbers from the disassembly, argument orders of the scene helpers unchecked. */

void ai_Unk4ded10(char *m)
{
    char sc[6][0x20] = {"duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3"};
    char ca[0x21c], cb[0x21c];
    char buf[0x400];
    char *a, *b, *w, *rec, *oa, *ob;
    int i, k, n, r, snd1, snd2, scene, hita, hitb, obj;
    int ids[2];

    r = (unsigned short)util_Random(100) - 5;
    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0) {
        err_Message("cut_MainDuell(): Could not create character");
        return;
    }
    if (ai_Unk6f0d8c != 0) {
        ai_Unk4debe0(m);
        return;
    }
    ai_Unk4e36a0();
    ai_Unk4e2e60(sc[*(char *)(m + 0x7c)]);
    ai_Unk4de5e0(10);
    ai_Unk4e3650();
    k = gm_ClockSeason(&ai_Time);
    if (k == 3)
        k = 0;
    else if (k == 0 || k == 2)
        k = 1;
    else if (k == 1)
        k = 2;
    d3_SetLightSet(k, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 1, 0);
    ai_Unk4cef80();
    if (k == 0)
        ai_Unk4e3620("cd2\\satisfaktion.mp3");
    else if (k == 1)
        ai_Unk4e3620("cd2\\DerSeuchenzug.mp3");
    else
        ai_Unk4e3620("cd2\\AmKuehlenGrabe.mp3");
    snd1 = sb_LoadSampleBank("duell_sfx.sbf", 0);
    snd2 = snd_LoadVoiceBank("duell.sbf", 0);
    scene = ai_Unk4e2bf0("cutscenes\\duell\\enter_DUELL.esc");
    memcpy(ca, a, 0x21c);
    memcpy(cb, b, 0x21c);
    ai_Unk92bca4 = (int)ca;
    ai_Unk92bca0 = (int)cb;
    ai_Unk6f0da8[0] = 0;
    ai_Unk6f0da8[1] = 0;
    unk_Unk4fbb80(ca + 0x10, d3_CurrentScene, 0, 0, 0, "dummy_DUELLANT_A_ENTRY");
    unk_Unk4fbb80(cb + 0x10, d3_CurrentScene, 0, 0, 0, "dummy_DUELLANT_B_ENTRY");
    oa = *(char **)(ca + 0x1b4 - 0x10 + 0x10);
    ob = *(char **)(cb + 0x1b4 - 0x10 + 0x10);
    if (oa == 0 || ob == 0) {
        err_Message("cut_MainDuell(): Could not create character");
        return;
    }
    ch_SetHandObject(oa, 2, "ob_PISTOLE");
    ch_SetHandObject(ob, 2, "ob_PISTOLE");
    *(unsigned char *)(oa + 0x240) = 4;
    *(unsigned char *)(ob + 0x240) = 4;
    for (i = 0; i < 3; i++) {
        d2_SetForm(ai_Unk60d368, 0);
        io_PrintText("$C");
        io_PrintText(*(char *)(m + 0x7c) * 3 + 0x10cb + i);
        sprintf(buf, "_DUELL_INTRO_NEU_0%d", i);
        n = ai_Unk4e32b0(-7, buf, 0, -1);
        while (n != 0 && sb_IsPlaying(n) != 0)
            ai_Unk4e3420(2);
    }
    ch_WalkToObject(oa, d3_FindObject(0, 0x100, 0, "dummy_DUELL_START_A"));
    ch_AddAction(oa, 0x35, d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_A"));
    ch_WalkToObject(ob, d3_FindObject(0, 0x100, 0, "dummy_DUELL_START_B"));
    ch_AddAction(ob, 0x35, d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_B"));
    d2_SetForm(ai_Unk60d368, 0);
    io_PrintText("$C");
    io_PrintText(0x10dd);
    ai_Unk4e3420(4);
    io_PrintText("$C");
    snd_PlayVoice(-7, "_DUELL_INTRO_BEGRUESSUNG_RUECKEN", 0, -1);
    ai_Unk4e3830(scene, 0);
    rec = (char *)ai_Unk4e50e0(*(int *)(ca + 0x10 - 0x10 + 0));
    /* the seconds: with the first round of kind 1 each side gets a shot object (callback 0x4de640, "kampf/schiessen_pistole") */
    hita = 0;
    hitb = 0;
    for (i = 0; i < 3 && ai_Unk6f0d94 == 0; i++) {
        ai_Unk4e3830(scene, scene);
        rec = (char *)ai_Unk4e50e0(*(int *)(m + 0x34));
        ai_Unk6f0da8[2] = 0;
        ai_Unk6f0da8[3] = 0;
        ai_Unk6f0da8[4] = 0;
        ai_Unk6f0da8[5] = 0;
        if (rec[0x94] == 2 || rec[0x94] == 3) {
            ai_Unk4de900(cb, "B");
            ai_Unk4de900(ca, "A");
        } else {
            ai_Unk4de900(ca, "A");
            ai_Unk4de900(cb, "B");
        }
        if ((ai_Unk6f0da8[2] != 0 || ai_Unk6f0da8[4] != 0) && (ai_Unk6f0da8[3] != 0 || ai_Unk6f0da8[5] != 0)) {
            n = ai_Unk4e52f0(9);
            d2_SetForm(ai_Unk60d368, 0);
            io_PrintText(n + 0x1113);
            snd_PlayVoice(-5, "_DUELL_INTRO_C2_DOKTOR", 0, n);
            ai_Unk4e3420(5);
        }
        d2_SetForm(ai_Unk60d368, 0);
        io_PrintText("$C");
    }
    /* result: who is down (the lower life 0x4fc8e0), the texts 0x111c / 0x111d, the damage 0x6f0da8 / 0x6f0da9 off the life */
    if (ai_Unk6f0d94 != 0)
        w = unk_Unk4fc8e0(cb) < unk_Unk4fc8e0(ca) ? cb : ca;
    else
        w = 0;
    if (w != 0 && w[0x166] != 0 && amt_FindOffice(w, buf, ids) != 0 && *(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        amt_SendOfficeMessage(ids[0], 0, 0, 4, 0xff);
        for (i = 0; i < 768; i++) {
            if (ai_Chars[i][2] == 6 || ai_Chars[i][2] == 7) {
                txt_Format(buf, 0x10c7, *(unsigned short *)w, (w[9] != 0) + 0x10c8);
                he_ShowNews(*(int *)(ai_Chars[i] + 4), -1, buf, 0, 0x5fe, &ch_EmptyName);
            }
        }
    }
    ai_Unk4e5360();
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        cm_SendRelations(*(int *)(cb + 4), *(int *)(ca + 4), 0xf, 1);
        cm_SendRelations(*(int *)(ca + 4), *(int *)(cb + 4), 0xf, 1);
    }
    ai_Unk4e3060();
    ai_Unk4e3250(oa);
    ai_Unk4e3250(ob);
    ai_Unk4e2d90(scene);
    ai_Unk4e2d40(scene);
    ai_Unk4e30c0();
    sb_UnloadSampleBank(snd1);
    sb_UnloadSampleBank(snd2);
    ai_Unk4cefa0();
    ai_Unk4e3640();
}
