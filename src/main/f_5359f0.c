/* 0x5359f0, the family choice of the character creation ("Menu\CHOOSECITY_HEADER" over the sky "Sky_Mittel_02"): nine template persons (0x5344f0: thief / gypsy 0 / 8, craftsmen 1 / 7, fighter 4, traders 3 / 5, priest 2 / 6) stand
   in the scene; the player clicks one for each of the four slots 0x75e180 [4] (father) and [5] (mother) (the clicked person 0x6f22e4 + 0x200 gets the slot in +0x23c and the dummy name "dummy_VATER" / "dummy_MUTTER" ..), the
   value stored in the slot is the profession group 0 .. 4; with both slots set the talent choice 0x534610 and the model choice 0x534b30 follow; returns 1 when the character was created (ROUGH: the dummy name table
   dn[6] of 0x30 bytes, entries 4 / 5 are the only ones used) */

int main_Unk5359f0(int unused)
{
    char dn[6][0x30] = { "dummy_GROSSVATER_VAETERLICH", "dummy_GROSSMUTTER_VAETERLICH", "dummy_GROSSVATER_MUETTERLICH", "dummy_GROSSMUTTER_MUETTERLICH", "dummy_VATER", "dummy_MUTTER" };
    int o1, o2, o3, o4, o5, o6, o7, o8, o9, f, run, k, sel, v, h, ret, scene, redraw;
    int r1[3], r2[3];
    unsigned char *fade;
    int *p;
    int q;

    ret = 0;
    redraw = 1;
    fade = 0;
    for (k = 0; k < 11; k++)
        ((int *)(unsigned)&full_data_75e180)[k] = 0;
    for (k = 0; k < 6; k++)
        ((int *)(unsigned)&full_data_75e180)[k] = -1;
    sv_ClearNamePlates();
    sv_ReleaseNamePlates();
    d3if_SelectScene(1, 0);
    p = (int *)U_I((unsigned)&ai_Unk5fb5b0);
    if (p != 0 && (q = p[0x3c8 / 4]) != 0 && *(int *)(q + 0x210) != 0)
        ((void (*)(int *, int, int, int))*(int *)(q + 0x210))(p, 4, q + 0x214, 0);
    scene = U_I((unsigned)&ai_Unk5f8438);
    if (U_I((unsigned)&ai_Unk633738) != 0)
        *(unsigned char *)(U_I((unsigned)&ai_Unk633738) + 0x1c70) = (*(unsigned char *)(U_I((unsigned)&ai_Unk633738) + 0x1c70) & 0xeb) | 8;
    o1 = main_Unk5344f0((int)"dummy_NACHT_UND_NEBEL_MANN", (int)"dieb_MANN2", 0);
    o2 = main_Unk5344f0((int)"dummy_NACHT_UND_NEBEL_FRAU", (int)"zigeunerin_FRAU", 8);
    o3 = main_Unk5344f0((int)"dummy_HANDWERKSKUNST_MANN", (int)"handwerker_MANN", 1);
    o4 = main_Unk5344f0((int)"dummy_HANDWERKSKUNST_FRAU", (int)"handwerkerin2_FRAU", 7);
    o5 = main_Unk5344f0((int)"dummy_KAMPF_MANN", (int)"offizier_SOLDAT", 4);
    o6 = main_Unk5344f0((int)"dummy_VERHANDELN_MANN", (int)"buerger3_DICKER", 3);
    o7 = main_Unk5344f0((int)"dummy_VERHANDELN_FRAU", (int)"buergerin2_FRAU", 5);
    o8 = main_Unk5344f0((int)"dummy_RHETORIK_MANN", (int)"priester_KUTTE", 2);
    o9 = main_Unk5344f0((int)"dummy_RHETORIK_FRAU", (int)"buergerin_FRAU", 6);
    f = d2_OpenForm(0, 0, "Menu\\CHOOSECITY_HEADER");
    d2_PlaceMainWindow(f, 1);
    k = 4;
    ai_Unk4e32f0("Sky_Mittel_02");
    d3_SetLightSet(4, 0, 0x3f800000, 1);
    if (U_I((unsigned)&full_data_624938) != -1) {
        d2_CloseView(U_I((unsigned)&full_data_624938));
        U_I((unsigned)&full_data_624938) = -1;
    }
    U_I((unsigned)&d2_CutsceneActive) = 0;
    run = sv_RunFrame((int)main_Unk5359f0, 0x12d6);
    if (run != 0) {
        do {
            if (redraw) {
                k = 4;
                do {
                    if (((int *)(unsigned)&full_data_75e180)[k] == -1) {
                        if (k < 6) {
                            d2_SetForm(f, 0);
                            io_PrintText((unsigned)"$C");
                            io_PrintText(0x1ae6, k + 0x1aea);
                            if (k % 2 == 0) {
                                r1[0] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xe4);
                                r1[1] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xe8);
                                r1[2] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xec);
                                r2[0] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xf0);
                                r2[1] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xf4);
                                r2[2] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xf8);
                            } else {
                                r1[0] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0xfc);
                                r1[1] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0x100);
                                r1[2] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0x104);
                                r2[0] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0x108);
                                r2[1] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0x10c);
                                r2[2] = *(int *)(U_I((unsigned)&ai_Unkfc2dec) + 0x110);
                            }
                            d3anim_FreePath(U_I((unsigned)&ai_Unkfc2dec));
                            U_I((unsigned)&d3t_CamMoving) = 0;
                            U_I((unsigned)&d3t_CamMove) = 0;
                            d3t_PlaceObjectV(U_I((unsigned)&ai_Unkfc2dec), r1, r2, 0x50, 8);
                        }
                        break;
                    }
                    k++;
                } while (k < 6);
                redraw = 0;
            }
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                U_I((unsigned)&sv_PopRequest) = 1;
            if (U_I((unsigned)&ai_ClickedObject) != 0 && (sel = *(int *)(U_I((unsigned)&ai_ClickedObject) + 0x200)) != 0) {
                if (*(int *)(sel + 0x23c) == -1) {
                    v = 0;
                    if (sel == o1 || sel == o2)
                        v = 2;
                    else if (sel == o6 || sel == o7)
                        v = 0;
                    else if (sel == o5)
                        v = 3;
                    else if (sel == o8 || sel == o9)
                        v = 4;
                    else if (sel == o3 || sel == o4)
                        v = 1;
                    if (k % 2 == 0) {
                        if (sel == o1 || sel == o6 || sel == o8 || sel == o3 || sel == o5) {
                            k = 4;
                            goto lset;
                        }
                    } else if (sel == o2 || sel == o7 || sel == o9 || sel == o4) {
                        k = 5;
lset:
                        ((int *)(unsigned)&full_data_75e180)[k] = v;
                        *(int *)(sel + 0x23c) = k;
                        h = d3_FindObject(0, 0x100, 0, dn[k]);
                        if (h != 0)
                            ch_WalkToObjectTurn(sel, h);
                    }
                }
                redraw = 1;
            }
            if (5 < k) {
                d2_SetForm(f, 0);
                io_PrintText((unsigned)"$C");
                d2_SetViewEnabled(f, 0);
                d3anim_FreePath(U_I((unsigned)&ai_Unkfc2dec));
                U_I((unsigned)&d3t_CamMoving) = 0;
                U_I((unsigned)&d3t_CamMove) = 0;
                d3t_PlaceObjectV(U_I((unsigned)&ai_Unkfc2dec), U_I((unsigned)&ai_Unkfc2dec) + 0x114, U_I((unsigned)&ai_Unkfc2dec) + 0x120, 0x50, 8);
                if (main_Unk534610() == 0) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 0;
                    d2_SetViewEnabled(f, 1);
                } else {
                    h = ai_Unk58b880(0xc6);
                    if (h == 0 || main_Unk534b30(*(unsigned char *)((unsigned)&init_PlayerProfile + 0x3)) == 0) {
                        if (ret == 0)
                            d2_SetViewEnabled(f, 1);
                    } else {
                        U_I((unsigned)&sv_PopRequest) = 1;
                        ret = 1;
                    }
                }
            }
            run = sv_RunFrame((int)main_Unk5359f0, 0x12d6);
        } while (run != 0);
        if (ret == 1) {
            fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x5a, 1);
            while ((*fade & 4) == 0)
                sv_RunFrame(0, 0xc6);
        }
    }
    if (o1 != 0)
        ch_Delete(o1);
    if (o2 != 0)
        ch_Delete(o2);
    if (o3 != 0)
        ch_Delete(o3);
    if (o4 != 0)
        ch_Delete(o4);
    if (o5 != 0)
        ch_Delete(o5);
    if (o6 != 0)
        ch_Delete(o6);
    if (o7 != 0)
        ch_Delete(o7);
    if (o9 != 0)
        ch_Delete(o9);
    if (o8 != 0)
        ch_Delete(o8);
    if (f != -1)
        d2_CloseView(f);
    if (scene != -1) {
        h = (int)evt_FindById(scene);
        if (h != 0)
            evt_Stop((EVTSCRIPT *)h);
        sv_Unk52bb70(scene);
    }
    if (*(int *)(U_I((unsigned)&ai_Unk5fb5b0) + 0xb0) != 0) {
        d3sm_Destroy(*(int *)(U_I((unsigned)&ai_Unk5fb5b0) + 0xb0));
        *(int *)(U_I((unsigned)&ai_Unk5fb5b0) + 0xb0) = 0;
    }
    ai_Unk4e3390();
    p = (int *)U_I((unsigned)&ai_Unk5fb5b0);
    if (p != 0 && (q = p[0x3c8 / 4]) != 0 && *(int *)(q + 0x294) != 0)
        ((void (*)(int *, int, int, int))*(int *)(q + 0x294))(p, 5, q + 0x298, 0);
    d3anim_FreePath(U_I((unsigned)&ai_Unkfc2dec));
    U_I((unsigned)&d3t_CamMoving) = 0;
    U_I((unsigned)&d3t_CamMove) = 0;
    if (fade != 0)
        d2_ReleaseFade(fade);
    return ret;
}
