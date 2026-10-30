/* 0x4e01e0, one execution (the message m: +0x34 the delinquent, +0x38 / +0x3c / +0x40 the others, +0x7c the way 0 .. 4: head, gallows (1, 2),
   shovel, cannon; +0xc the owner): the scene "Richtplatz.ed3" with a random sky, the names of the four in the text, the intro, the
   way's own scene, the pause dur[kind], the exit scene; the delinquent's life goes down by (life + 5). ROUGH */

void ai_Unk4e01e0(char *m)
{
    int dur[5] = {0x36b0, 0x2ee0, 0x2ee0, 0x3a98, 0x2710};
    char buf[0x100];
    char *v, *p1, *p2, *p3, *nm;
    int w;

    w = ai_Unk4e52f0(3);
    v = (char *)gm_FindChar(*(int *)(m + 0x34));
    if (v == 0)
        return;
    p1 = (char *)gm_FindChar(*(int *)(m + 0x38));
    p2 = (char *)gm_FindChar(*(int *)(m + 0x3c));
    p3 = (char *)gm_FindChar(*(int *)(m + 0x40));
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    ai_Unk4e2e60("Richtplatz.ed3");
    d3_SetLightSet(0, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 0, 0);
    if (w == 1)
        ai_Unk4e32f0(ai_Unk623ac0 + ai_Unk4e52f0(3) * 0x4c);
    else if (w == 2)
        ai_Unk4e32f0(ai_Unk623ac0 - 0xe8 + ai_Unk4e52f0(3) * 0x4c);
    else
        ai_Unk4e32f0(ai_Unk623ac0 - 0x218 + ai_Unk4e52f0(4) * 0x4c);
    ai_Unk4e3620("cd2\\ArmerDelinquent.mp3");
    if (w == 1)
        ai_Unk4e33d0(0x1f4);
    nm = v != 0 ? (char *)unk_Unk4fb390(*(unsigned short *)v) : (char *)unk_Unk4fb330((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf, "%s ", nm + 4);
    nm = p1 != 0 ? (char *)unk_Unk4fb390(*(unsigned short *)p1) : (char *)unk_Unk4fb330((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf + strlen(buf), "%s ", nm + 4);
    nm = p2 != 0 ? (char *)unk_Unk4fb390(*(unsigned short *)p2) : (char *)unk_Unk4fb330((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf + strlen(buf), "%s ", nm + 4);
    nm = p3 != 0 ? (char *)unk_Unk4fb390(*(unsigned short *)p3) : (char *)unk_Unk4fb330((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf + strlen(buf), "%s", nm + 4);
    ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-create", buf);
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-intro.", &ch_EmptyName));
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0])
        gm_AddHealth(*(unsigned short *)v, (int)-(*(float *)(v + 0x1c) + 5.0f));
    switch (m[0x7c]) {
    case 0:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-kopf.e", &ch_EmptyName);
        break;
    case 1:
    case 2:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-galgen", &ch_EmptyName);
        break;
    case 3:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-shaufe", &ch_EmptyName);
        break;
    case 4:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-kanone", &ch_EmptyName);
        break;
    }
    ai_Unk4e3570(dur[m[0x7c]]);
    ai_Unk4e3060();
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-exit.e", &ch_EmptyName));
    if (v != 0)
        ai_Unk4e33f0();
    ai_Unk4e3390();
    ai_Unk6f0da4 = 0;
    ai_Unk4e30c0();
    d2_CutsceneActive = 0;
    ai_Unk4e3640();
}
