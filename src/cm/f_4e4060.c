/* 0x4e4060, runs a scene party m (the main function of the cutscene kinds: +0 id, +8 the kind, +0xc the owner, +0x26 flags (4 running, 0x20 own view,
   0x40 no fade), +0x2c the window, +0x78 the seed): saves the view state (0x5f1290, 0x631064, the viewport 0xfb9060 .., the sky / fog 0x75fb80 .., the objects of
   0xf8f080 that are shown), clears the scene table 0x92c600, decides the replay mode (0x6f0d8c: the run of the kind 6 members is only watched), fades out
   ("BLACK", 0x5a ticks), opens the window "cutscenes\\announcement" (or "..\\<own view>"), calls the run function of the kind (0x92c500, with m),
   closes everything again, fades in, and gives every member back its field +0x20c = -1 (0x4c3d90). ROUGH: the viewport arithmetic (0x43b950) and the
   decisions of the replay mode are only roughly in the order of the code */

void ai_Unk4e4060(char *m)
{
    char buf[0x240];
    float sv_fog;
    int sv_time, sv_view[4], sv_sky[6], sv_byte, shown, replay, i, j, n, win_old, id, ours;
    char *c;
    unsigned int fl;

    sv_fog = cm_SpeedFactor;
    sv_time = ai_Unk631064;
    sprintf(buf, "Exec Cutscene(Mainfunc start) <Type = %i> <id = %i>", (unsigned char)m[8], *(int *)m);
    shown = 0;
    sv_view[2] = ai_Unkfb9068;
    sv_view[0] = ai_Unkfb9060;
    sv_view[1] = ai_Unkfb9064;
    sv_view[3] = ai_Unkfb906c;
    ai_Unk925510 = d3_CurrentScene;
    ai_Unk92cac0 = evt_NoGroup;
    ai_Unk6f0d7c = 0;
    ai_Unk4cef80();
    AI_ZERO_AT(offset ai_Unk92c600, 0x480);  /* @vc6 clang's MS asm cannot take the offset of an incomplete array */
    ai_Unk6f0d8c = 0;
    m[0x26] |= 4;
    if (sv_FrameMask & 0x200000)
        m[0x26] |= 0x40;
    ai_Unk60d360 = 0x717c6;
    if (m[0x26] & 0x40)
        ai_Unk60d360 = 0x716c6;
    replay = 0;
    if (ai_GameFlags & 8) {
        ours = 1;
        n = *(unsigned char *)(m + 0x30);
        for (i = 0; i < n; i++) {
            id = *(int *)(m + 0x34 + i * 4);
            if (id != -1 && (c = (char *)gm_FindChar(id)) != 0 && c[2] == 6) {
                ours = 0;
                break;
            }
        }
        for (j = 0; j < n; j++) {
            id = *(int *)(m + 0x34 + j * 4);
            if (id != -1 && (c = (char *)gm_FindChar(id)) != 0 && c[2] == 6)
                return;
        }
    }
    if (m[0x26] & 2) {
        if (ai_GameFlags & 8) {
            if (ours != 0)
                replay = 1;
        } else if ((ai_GameFlags & 4) && ai_Unk4e4870(ai_CharInts[ai_PlayerId][0], m) == 0)
            replay = 1;
    }
    if (replay != 0)
        ai_Unk6f0d8c = 1;
    if (ai_Unk6f0d8c == 0) {
        fl = (sv_FrameMask & ~8) | 0x10080;
        ai_Unk6f0da0 = (char *)d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 1);
        while ((*ai_Unk6f0da0 & 4) == 0)
            sv_RunFrame(0, fl);
        sv_sky[0] = ai_Unk75fb80;
        sv_sky[1] = ai_Unk75fb84;
        sv_sky[2] = ai_Unk75fb88;
        sv_sky[3] = ai_Unk75fb90;
        sv_sky[4] = ai_Unk75fb94;
        sv_sky[5] = ai_Unk75fb98;
        if (ai_GameFlags & 8)
            ai_Unk4e3fe0();
        tb_OpenFunction(0x46, ai_Unk4e3210);
        sv_byte = d3_FogByDistance;
        d3_FogByDistance = 0;
        cm_SpeedFactor = 0.6f;
        d3o_FogScale = 1.0f;
        ai_Unk631064 = 0;
        d2_SetPointerSprite(0, 0);
        sv_ClearSelection();
        d2_ClearPointerItems();
        sv_ResetScreenClip();
        ai_Unkf8aae0[0] = 0;
        for (i = 0, shown = 0; i < 0x8c400 / 0x2ec; i++) {
            if (*(int *)(d2_ObjsC + i * 0x2ec + 0x18) != 0 && *(int *)(d2_ObjsC + i * 0x2ec + 0x38) == 0) {
                ai_Unk92bee0[shown] = i;
                *(int *)(d2_ObjsC + i * 0x2ec + 0x38) = 1;
                shown++;
            }
        }
        ((int (*)())d3io_NoopCallback)();
        ai_Unk4d8b30();
    } else if (ai_Unk6f0d90 == 0)
        ai_Unk4e36a0(m);
    win_old = ai_Unk60d368;
    *(int *)(m + 0x2c) = d2_OpenForm(m[8] == 2 ? "cutscenes\\prozess_main" : (m[0x26] & 0x20) ? "cutscenes\\announcement_cinemascope" : "cutscenes\\announcement", 0, 0);
    ai_Unk60d368 = *(int *)(m + 0x2c);
    ((void (*)(int, int))d2_PlaceMainWindow)(ai_Unk60d368, 3);
    if (ai_Unk6f0d8c != 0)
        d2_SetViewEnabled(ai_Unk60d368, 0);
    ai_Unk4e52a0(*(int *)(m + 0x78));
    sprintf(buf, "Exec Cutscene(Mainfunc) <Type = %i>", (unsigned char)m[8]);
    ai_Unk6f0d90 = (int *)m;
    ((void (*)())ai_Unk92c500[(unsigned char)m[8] * 5])(m);
    d2_CloseView(*(int *)(m + 0x2c));
    ai_Unk60d368 = win_old;
    if (ai_Unk6f0d8c == 0) {
        ai_Unk4d8c40();
        ai_Unk631064 = sv_time;
        if (ai_GameFlags & 8)
            ai_Unk4e4020();
        ai_Unk60d364 = 10;
        tb_CloseFunction(ai_Unk4e3210);
        for (i = 0; i < shown; i++)
            *(int *)(d2_ObjsC + ai_Unk92bee0[i] * 0x2ec + 0x38) = 0;
        ai_Unk4e36a0(m);
    } else
        ai_Unk4e36a0(m);
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        for (i = 0; i < 16; i++) {
            *(int *)(ai_Unk92cae0[i] + 4) = 0;
            *(int *)(ai_Unk92cae0[i]) = 0;
            *(int *)(ai_Unk92cae0[i] + 8) = 0;
            id = *(int *)(m + 0x34 + i * 4);
            if (id != -1 && (c = (char *)gm_FindChar(id)) != 0) {
                cm_BeginFieldBlock(c, *(int *)(c + 4));
                id = -1;
                cm_AddFieldValue(4, 1, (unsigned short)(c - cm_FieldObject + 0x20c), &id);
                cm_SendFieldValues();
            }
        }
    }
    id = *(int *)(m + 0xc);
    if (id != -1 && (c = (char *)gm_FindChar(id)) != 0) {
        id = -1;
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldValue(4, 1, (unsigned short)(c - cm_FieldObject + 0x20c), &id);
        cm_SendFieldValues();
    }
    ai_Unk6f0d90 = 0;
    evt_NoGroup = ai_Unk92cac0;
}
