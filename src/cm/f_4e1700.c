/* 0x4e1700, a bankruptcy (the message m: +0x34 the person): the scene "Pleite.ed3" with "cutscenes\\pleite\\enter_Pleite.esc", a sky by the age
   0x770ae4, the voices "_PLEITE_HS" 0 .. 5 (pauses 0x9c4 / 0x1f4 / 0x251c / 0x1194), the text 0x1ab1 (0x3fda with the flag 0x6f23f8 & 4)
   as a message (0x4e5470); with the flag & 8 the person is removed (+8 = 0, 0x53bb20). ROUGH */

int ai_Unk4e1700(char *m)
{
    char buf[0x200];
    char big[0x1c00];
    char *a;
    int ages[6] = {8, 10, 12, 15, 18, 20};
    int i, stage, sc, snd, rc;

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
    snd = snd_LoadVoiceBank("pleite.sbf", 1);
    ai_Unk4e3620("cd2\\DerSeuchenzug.mp3");
    ai_Unk4e2e60("Pleite.ed3");
    d3_SetLightSet(stage, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 0, 0);
    ai_Unk4e32f0("sky_dunkel_01");
    sprintf(buf, "%i", stage);
    sc = ai_Unk4e2c30("cutscenes\\pleite\\enter_Pleite.esc", buf);
    rc = ai_Unk4e3490(0x1770, 0, 0);
    if (rc == 0) {
        ai_Unk4e32d0(-8, "_PLEITE_HS", 0, 0);
        ai_Unk4e32d0(-8, "_PLEITE_HS", 1, 0x9c4);
        ai_Unk4e32d0(-8, "_PLEITE_HS", 2, 0x1f4);
        rc = ai_Unk4e3490(0x3e80, 0, 0);
        if (rc == 0) {
            ai_Unk4e33d0(0xfa);
            rc = ai_Unk4e3490(0x2af8, 0, 0);
            if (rc == 0) {
                ai_Unk4e32d0(-8, "_PLEITE_HS", 3, 0x251c);
                ai_Unk4e32d0(-8, "_PLEITE_HS", 4, 0x1194);
                ai_Unk4e32d0(-8, "_PLEITE_HS", 5, 0x1f4);
                ai_Unk4e3490(0x5dc0, 0, 0);
            }
        }
    }
    d2_CutsceneActive = 0;
    if ((ai_GameFlags & 4) != 0)
        txt_Format(buf, 0x3fda, *(unsigned short *)a);
    else
        txt_Format(buf, 0x1ab1, *(unsigned short *)a);
    ai_Unk4e5470(buf, 0);
    ai_Unk4e3060();
    ai_Unk4e2d90(sc);
    ai_Unk4e2c70(sc);
    ai_Unk4e3390();
    ai_Unk4e33f0();
    ai_Unk4e30c0();
    snd_ClearQueue();
    sb_UnloadSampleBank(snd);
    ai_Unk4e3640();
    ai_Unk6f0da4 = 0;
    if ((ai_GameFlags & 8) != 0) {
        a[8] = 0;
        ai_GameOver = 1;
        sv_PopTo((unsigned)&main_GameLoop);
        return 0;
    }
    gm_Unk506e70(a, big, 0);
    return 0;
}
