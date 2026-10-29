/* 0x52ae70, "sv_HandleObjects": turns the object under the cursor (0x6f22e8, found by 0x52a680) into the selection state: the quick-jump contact name 0x75e8d4 (panel flag 0x75e8c0) is looked
   up as an object when the panel shows the remembered subject, the hint objects 0x760458 .. 0x760460, the highlight (0x4118a0, 0x6f22ec) and the group selection (0x7662c0 / 0x6f2334) are
   set; ROUGH */

void sv_Unk52ae70(void)
{
    unsigned short *g;
    char *o, *cls, *s, *t;
    int subj, n, found;
    char msg[0x80];

    found = 0;
    cls = 0;
    SV_I((unsigned)&full_data_760458) = 0;
    SV_I((unsigned)&full_data_76045c) = 0;
    SV_I((unsigned)&ai_ClickedObject) = 0;
    SV_I((unsigned)&full_data_760460) = 0;
    SV_I((unsigned)&full_data_6f22fc) = 0;
    if (SV_I((unsigned)&ai_Unk7662c0) != 0 && *(char *)(SV_I((unsigned)&ai_Unk7662c0) + 0x188) == 0)
        SV_I((unsigned)&ai_Unk7662c0) = 0;
    if (SV_I((unsigned)&sv_PanelFlag) != 0) {
        subj = SV_I((unsigned)&ai_PlayerBuilding);
        if (subj == 0)
            subj = SV_I((unsigned)&ai_FallbackBuilding);
        if (subj == SV_I((unsigned)&sv_PanelObject)) {
            if (SV_I((unsigned)&sv_PanelShown) == SV_I((unsigned)&sv_PanelRecord)
                || (SV_I((unsigned)&ai_FallbackBuilding) != 0 && SV_I((unsigned)&sv_PanelShown) == 0 && SV_I((unsigned)&sv_PanelRecord) != 0 && *(short *)SV_I((unsigned)&sv_PanelRecord) == 0xfd)) {
                if (SV_B((unsigned)&sv_PanelText) != 0) {
                    o = (char *)d3_FindObject(0, 0x60, 0, (unsigned)&sv_PanelText);
                    if (o != 0) {
                        found = 1;
                        SV_I((unsigned)&sv_PanelFlag) = 0;
                        SV_C((unsigned)&ai_Unk6f22e8) = o;
                        goto lf73;
                    }
                    sprintf(msg, "sv_HandleObjects(): Could not find QuickJump.ContactName: %s", (char *)(unsigned)&sv_PanelText);
                }
                SV_I((unsigned)&sv_PanelFlag) = 0;
            }
lf73:
            if (SV_I((unsigned)&sv_PanelRecord) == 0 || *(short *)SV_I((unsigned)&sv_PanelRecord) == 0xfd) {
                if (SV_B((unsigned)&sv_PanelText) != 0) {
                    o = (char *)d3_FindObject(0, 0x60, 0, (unsigned)&sv_PanelText);
                    if (o != 0) {
                        found = 1;
                        SV_I((unsigned)&sv_PanelFlag) = 0;
                        SV_C((unsigned)&ai_Unk6f22e8) = o;
                        goto lfe4;
                    }
                    sprintf(msg, "sv_HandleObjects(): Could not find QuickJump.ContactName: %s", (char *)(unsigned)&sv_PanelText);
                }
                SV_I((unsigned)&sv_PanelFlag) = 0;
            }
        }
    }
lfe4:
    if ((SV_I(((unsigned)&dm_L + 0xc)) != 0 && SV_I((unsigned)&d3t_CamMove) == 0 && SV_S((unsigned)&dm_L) > SV_I((unsigned)&sv_ViewRectLeft) && SV_S((unsigned)&dm_L) < SV_I((unsigned)&sv_ViewRectRight)
         && SV_I((unsigned)&sv_ViewRectTop) < SV_S(((unsigned)&dm_L + 0x2)) && SV_S(((unsigned)&dm_L + 0x2)) < SV_I((unsigned)&sv_ViewRectBottom) && d2_HitWindow == -1 && SV_I((unsigned)&d2_HitUnderPointer) == -1)
        || found) {
        if (SV_C((unsigned)&ai_Unk6f22e8) == 0) {
            if (SV_B((unsigned)&full_data_6f2338) == 0) {
                sv_Unk52adb0();
                return;
            }
        } else {
            SV_I((unsigned)&full_data_760458) = SV_I((unsigned)&full_data_6f22f4);
            SV_I((unsigned)&full_data_76045c) = SV_I((unsigned)&full_data_6f22f0);
            SV_I((unsigned)&full_data_760460) = SV_I((unsigned)&ai_Unk6f22f8);
            SV_I((unsigned)&full_data_6f22fc) = SV_I((unsigned)&full_data_6f2300);
            if (SV_I((unsigned)&full_data_6f22f0) != 0)
                cls = ai_Goods + *(unsigned short *)SV_I((unsigned)&full_data_6f22f0) * 0x41;
            if (memcmp(SV_C((unsigned)&ai_Unk6f22e8), "tp_TUER", 8) == 0 || (cls != 0 && *cls == 6) || SV_I((unsigned)&full_data_6f2300) != 0)
                found = 1;
            else
                found = 0;
            SV_C((unsigned)&ai_ClickedObject) = SV_C((unsigned)&ai_Unk6f22e8);
            if ((SV_C((unsigned)&ai_Unk6f22e8)[0x211] & 1) != 0 && !found) {
                d3t_PlaceAtObject(SV_C((unsigned)&ai_Unk6f22e8), SV_I((unsigned)&ai_CameraStep));
                SV_C((unsigned)&full_data_6f22ec) = SV_C((unsigned)&ai_Unk6f22e8);
                if (SV_C((unsigned)&ai_Unk6f22e8) != 0 && (n = *(int *)(SV_C((unsigned)&ai_Unk6f22e8) + 0x1d4)) != 0 && *(int *)(n + 0x108) != 0)
                    ((int (*)())*(int *)(n + 0x108))(SV_C((unsigned)&ai_Unk6f22e8), 2, n + 0x10c, 0);
                SV_I((unsigned)&full_data_6f22a8) = SV_I((unsigned)&sv_FrameDepth);
            }
            SV_C((unsigned)&full_data_6f23e0) = SV_C((unsigned)&ai_Unk6f22e8);
            if (SV_I((unsigned)&ai_Unk6f22f8) != 0 && *(char *)(SV_I((unsigned)&ai_Unk6f22f8) + 8) != 0) {
                g = (unsigned short *)SV_I((unsigned)&ai_Unk6f22f8);
                n = unk_Unk503530(ai_PlayerId, 0, 0, g);
                sv_Unk52ae40();
                if ((n & 0x800) != 0 && (*(char *)((char *)g + 0x165) != 0x1c || (g[0xda] & 1) == 0)) {
                    SV_I((unsigned)&ai_Unk7662c0) = (int)g;
                    SV_B((unsigned)&ai_Selected + *g * 0x21c) = 1;
                    SV_I((unsigned)&ai_Unk6f2334) = 1;
                    SV_I((unsigned)&ai_Unk631078) = SV_I(((unsigned)&ai_Unk770cec + 0x18) + *g * 0x21c);
                    snd_PlayWorkerClick(g);
                }
                SV_I((unsigned)&ai_Unk631078) = SV_I(((unsigned)&ai_Unk770cec + 0x18) + *(unsigned short *)SV_I((unsigned)&ai_Unk6f22f8) * 0x21c);
            }
            if (SV_I((unsigned)&ai_Unk6f2334) == 0 && SV_B((unsigned)&full_data_6f2338) == 0 && SV_I((unsigned)&full_data_6f22f4) != 0 && SV_I((unsigned)&ai_Unk75ec5c) == 0)
                SV_I((unsigned)&ai_Unk6f2304) = SV_I((unsigned)&ai_Unk6f22e8);
            SV_I((unsigned)&ai_Unk75ec60) = SV_I((unsigned)&full_data_6f22f4);
            if (SV_I((unsigned)&full_data_6f22f4) != 0)
                SV_I((unsigned)&ai_Unk6f2304) = SV_I((unsigned)&ai_Unk6f22e8);
            SV_I((unsigned)&ai_Unk75ec5c) = SV_I((unsigned)&full_data_6f22f0);
            if (SV_I((unsigned)&full_data_6f22f0) != 0) {
                SV_I((unsigned)&ai_Unk6f2304) = SV_I((unsigned)&ai_Unk6f22e8);
                if (ai_Goods[*(unsigned short *)SV_I((unsigned)&full_data_6f22f0) * 0x41] == 0x1d)
                    sv_Unk52ae40();
            }
        }
    }
}
