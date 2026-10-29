/* 0x52d940, the mouse click handler of the game screen (fl: 0x800 right button, 0x100 left button, 8 = key flag): plays "MausclickRechts_s_01" for a right click that changes something, closes a
   dialog (0x6f262c: the kinds 0x11 / 0x26 / 0x69 set 0x6f2630, 0x89 goes to 0x575860, the others to 0x53dcb0), routes a click on a shown widget (0x6faa84 table of 3 ints: widget, +0x70 of the object),
   and for the right button resets the selection / the view to the stored one (the "RaumWechseln_s" sound when a room is entered through the 0xfd item); the left button hands over to 0x52a050
   and updates the shown subject (0x6f23e4 / 0x6f23e8) through 0x52e190; ROUGH */

void sv_Unk52d940(unsigned fl)
{
    char c;
    int ok, i, *p, v, who;
    short *it;

    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) != (int)ai_GameFrame || SV_I((unsigned)&ai_Unk6f2334) != 0 || SV_I((unsigned)&ai_Unk75ec5c) != 0 || SV_I((unsigned)&ai_Unk7662c0) != 0 || SV_I((unsigned)&ai_Unk75ec60) != 0))
        sb_PlaySample(SV_I((unsigned)&snd_SystemBank), "MausclickRechts_s_01", 1, 0x3f, 0x7f, 0, 0);
    if (SV_I((unsigned)&full_data_6f262c) != 0) {
        if (SV_I(((unsigned)&dm_L + 0x20)) == 0 && *(char *)(unsigned)&kb_Last != 0x1c)
            return;
        if (*(char **)SV_I((unsigned)&full_data_6f262c) != 0) {
            c = **(char **)SV_I((unsigned)&full_data_6f262c);
            if (c == 0x11 || c == 0x26 || c == 0x69) {
                SV_I((unsigned)&full_data_6f2630) = 1;
                goto l9f2;
            }
            if (c == (char)-0x79) {
                ai_Unk575860();
                goto l9f2;
            }
        }
        he_Unk53dcb0(0);
l9f2:
        di_ClearEvents(0x10);
        if (*(char *)(unsigned)&kb_Last != 0x1c)
            return;
        *(char *)(unsigned)&kb_Last = 0;
        return;
    }
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && ai_Unk5fe710 != -1) {
        i = 0;
        p = (int *)(unsigned)&full_data_6faa84;
        do {
            if (*p != -1 && p != (int *)4 && ai_Unk5fe710 == *(int *)(SV_I((unsigned)&d2_Objs) + *p * 0x2ec)) {
                *(int *)(SV_I((unsigned)&full_data_6faa80 + i * 12) + 0x70) = -1;
                SV_I((unsigned)&full_data_6f2630) = 0;
                di_ClearButtons();
                break;
            }
            p += 3;
            i++;
        } while ((int)p < ((unsigned)((unsigned)full_data_6faa84 + 0xbc) + 0x4));
    }
    if ((fl & 0x800) == 0)
        goto l72;
    ai_Unk591170();
    anm_Update();
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && SV_I((unsigned)&ai_Unk75ec60) != 0 && SV_I(((unsigned)&ai_Unk75f280 + 0x500) + SV_I((unsigned)&sv_FrameDepth) * 8) == (int)ai_GameFrame) {
        sv_Unk52adb0();
        goto l72;
    }
    if (SV_I((unsigned)&ai_PlayerBuilding) == 0) {
        if (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_I((unsigned)&sv_PanelShown) != 0 && SV_I((unsigned)&full_data_6f22fc) != 0 && SV_I((unsigned)&ai_Unk6f2334) == 0 && SV_B((unsigned)&full_data_6f2338) == 0) {
            SV_I((unsigned)&sv_PopRequest) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_FrameBase) + 1;
            d3_SetObjectPos(ai_Unkfc2dec, (unsigned)&ai_Unk75fb80);
            d3_SetObjectRot(ai_Unkfc2dec, (unsigned)&ai_Unk75fb90);
            goto lbb6;
        }
    } else if (SV_I((unsigned)&full_data_6f22fc) != 0 && SV_I((unsigned)&ai_Unk6f2334) == 0 && SV_B((unsigned)&full_data_6f2338) == 0) {
        sb_PlaySample(SV_I((unsigned)&full_data_6f2410), "RaumWechseln_s", 1, 0x3f, 0x7f, 0, 0);
        *(short *)(SV_I((unsigned)&ai_PlayerBuilding) + 0x29) = *(short *)SV_I((unsigned)&full_data_6f22fc);
        SV_I((unsigned)&sv_PopRequest) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_FrameBase) + 1;
        d3_SetObjectPos(ai_Unkfc2dec, (unsigned)&ai_Unk75fb80);
        d3_SetObjectRot(ai_Unkfc2dec, (unsigned)&ai_Unk75fb90);
lbb6:
        SV_I((unsigned)&ai_Unk6f22cc) = 0;
        SV_I((unsigned)&ai_Unk6f22d0) = 0;
    }
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && (SV_B((unsigned)&full_data_6f2338) != 0 || (SV_I((unsigned)&ai_Unk75ec5c) != 0 && ai_Goods[*(unsigned short *)SV_I((unsigned)&ai_Unk75ec5c) * 0x41] == 0x1d))) {
        sv_Unk52adb0();
        di_ClearEvents(0x10);
    }
    if ((SV_I((unsigned)&ai_PlayerBuilding) != 0 && ai_Unk576340(0x1a, SV_I((unsigned)&ai_PlayerBuilding), 0, *(unsigned short *)SV_I((unsigned)&sv_PanelShown)) == 0)
        || (SV_I((unsigned)&ai_FallbackBuilding) != 0 && ai_Unk576340(0x1c, SV_I((unsigned)&ai_FallbackBuilding), 0, 0) == 0))
        di_ClearAxes();
    if (SV_I((unsigned)&ai_Unk6f2334) != 0 || SV_B((unsigned)&full_data_6f2338) != 0 || SV_I(((unsigned)&dm_L + 0x20)) == 0)
        goto l72;
    ok = 0;
    if (SV_I((unsigned)&d3t_CamMove) != 0) {
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0)
            goto l72;
        if (d3_VecEqual((char *)(unsigned)&d3_Unk1466620, (char *)(unsigned)&ai_Unk75fb80, 0.1f) == 0) {
            d3anim_FreePath(ai_Unkfc2dec);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
        } else {
            d3anim_FreePath(ai_Unkfc2dec);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
            ok = 1;
            d3_SetObjectPos(ai_Unkfc2dec, (unsigned)&ai_Unk75fb80);
            d3_SetObjectRot(ai_Unkfc2dec, (unsigned)&ai_Unk75fb90);
        }
    }
    if (SV_I((unsigned)&sv_FrameDepth) <= SV_I((unsigned)&sv_FrameBase) || ok) {
        if (d3_VecEqual(ai_Unkfc2dec + 0x84, (char *)(unsigned)&ai_Unk75fb90, 0.1f) != 0 || d3_VecEqual(ai_Unkfc2dec + 0x4c, (char *)(unsigned)&ai_Unk75fb80, 0.1f) != 0 || ok) {
            if (SV_I((unsigned)&ai_PlayerBuilding) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&sv_PanelShown) == 0) {
                if (SV_I((unsigned)&sv_FrameDepth) == SV_I((unsigned)&sv_FrameBase) && SV_I((unsigned)&ai_Unk75ec5c) != 0)
                    sv_Unk52adb0();
                goto l72;
            }
            it = 0;
            if (SV_I((unsigned)&sv_PanelShown) == 0) {
                if (SV_I((unsigned)&ai_FallbackBuilding) != 0 || SV_I((unsigned)&ai_PlayerBuilding) != 0) {
                    SV_I((unsigned)&ai_FallbackBuilding) = 0;
                    SV_I((unsigned)&ai_PlayerBuilding) = 0;
                    goto l1a;
                }
            } else {
                if (SV_I((unsigned)&ai_PlayerBuilding) == 0) {
                    if (SV_I((unsigned)&ai_FallbackBuilding) == 0)
                        goto l1a;
                    if (ai_Unk576340(0x1c, SV_I((unsigned)&ai_FallbackBuilding), 0, 0) == 1) {
                        c = SV_C(((unsigned)&ai_Selected + 0x29))[ai_PlayerId * 0x21c];
                        v = SV_I((unsigned)&ai_FallbackBuilding);
                        goto lbd;
                    }
                } else {
                    if (ai_Unk576340(0x1a, SV_I((unsigned)&ai_PlayerBuilding), 0, *(unsigned short *)SV_I((unsigned)&sv_PanelShown)) == 1) {
                        c = SV_C(((unsigned)&ai_Selected + 0x29))[ai_PlayerId * 0x21c];
                        v = SV_I((unsigned)&ai_PlayerBuilding);
lbd:
                        if (c == 0 && v != 0 && *(unsigned short *)SV_I((unsigned)&sv_PanelShown) == 0xf2)
                            it = (short *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(v);
                    }
                }
                if (SV_I((unsigned)&ai_PlayerBuilding) == 0 || it == 0 || (int)it == SV_I((unsigned)&sv_PanelShown)) {
l1a:
                    SV_I((unsigned)&sv_PanelShown) = 0;
                } else
                    ai_Unk59e0a0(-1, *it);
            }
            SV_I((unsigned)&sv_PopRequest) = 1;
            if (SV_I((unsigned)&ai_PlayerBuilding) != 0 && SV_I((unsigned)&sv_PanelShown) == 0)
                SV_I((unsigned)&ai_PlayerBuilding) = 0;
            d3_SetObjectPos(ai_Unkfc2dec, (unsigned)&ai_Unk75fb80);
            d3_SetObjectRot(ai_Unkfc2dec, (unsigned)&ai_Unk75fb90);
            SV_I((unsigned)&ai_Unk6f22cc) = 0;
            SV_I((unsigned)&ai_Unk6f22d0) = 0;
            goto l72;
        }
        if (SV_I((unsigned)&ai_PlayerBuilding) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
            sv_Unk528140();
    } else {
        SV_I((unsigned)&sv_PopRequest) = 1;
        if (SV_I((unsigned)&full_data_6f22a8) < SV_I((unsigned)&sv_FrameDepth) - 1)
            goto l72;
        if (SV_I((unsigned)&ai_PlayerBuilding) != 0 || SV_I((unsigned)&ai_FallbackBuilding) != 0)
            sv_Unk528140();
        SV_I((unsigned)&full_data_6f22a8) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_PopRequest);
    }
    if (SV_I((unsigned)&full_data_6f22ec) != 0) {
        v = *(int *)(SV_I((unsigned)&full_data_6f22ec) + 0x1d4);
        if (v != 0 && *(int *)(v + 0x18c) != 0)
            ((int (*)())*(int *)(v + 0x18c))(SV_I((unsigned)&full_data_6f22ec), 3, v + 400, 0);
        SV_I((unsigned)&full_data_6f22ec) = 0;
    }
    sv_Unk52adb0();
l72:
    if ((fl & 0x100) != 0)
        sv_UpdatePanel();
    who = SV_I((unsigned)&ai_PlayerBuilding);
    if (who == 0 && (who = SV_I((unsigned)&ai_FallbackBuilding)) == 0)
        who = 0;
    if ((fl & 8) != 0 && who != 0) {
        it = (short *)SV_I((unsigned)&sv_PanelShown);
        if (it == 0)
            it = 0;
        SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, who, it, 0);
        if (*(unsigned short *)(who + 0x27) == ai_PlayerId || (SV_I((unsigned)&full_data_6f2320) & 0x1000) != 0)
            sv_Unk525ff0();
    }
    if (SV_I((unsigned)&full_data_6f23e4) != who || SV_I((unsigned)&full_data_6f23e8) != SV_I((unsigned)&sv_PanelShown) || SV_I((unsigned)&ai_Unk6f2318) != 0) {
        SV_I((unsigned)&ai_BuildingPerms) = unk_Unk503530(ai_PlayerId, who, SV_I((unsigned)&sv_PanelShown), 0);
        sv_ReleaseNamePlates();
        SV_I((unsigned)&full_data_6f23e8) = SV_I((unsigned)&sv_PanelShown);
        SV_I((unsigned)&ai_Unk6f2318) = 0;
        SV_I((unsigned)&full_data_6f23e4) = who;
    }
}
