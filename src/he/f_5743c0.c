/* 0x5743c0, the run of the event 0xa0 (the trial): a pending handle +0x84 is awaited (0x4c3ae0); -2 / -1 free; state 0: the
   building +0x10 (0x502020) and its group (0x4ffe90 ([building +0x5d], 1, 0, 0x27f)) are taken, the due clock + 2 hours
   (0x4fe140); with a group the texts 0x4205 / 0x5fc (0x5176d0 / 0x53dee0) and the scene (0x511dc0, 0x4287b0, 0x429ab0) run;
   then +0x58 = 0 and state 1; state 1: nothing while paused (+0x78 & 4), else the trial 0x5741f0 and scheduled with -1 */

void he_Unk5743c0(int ev)
{
    char text[0x400];
    char *bld, *grp, *ch;
    int sc;

    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (HM_I(ev, 0x70) == 0) {
        bld = (char *)gm_FindBuildingById(HM_I(ev, 0x10));
        grp = 0;
        if (bld != 0)
            grp = (char *)gm_Query(HM_I(bld, 0x5d), 1, 0, 0x27f);
        gm_AddTime(ev + 0x52, 2, 0, 0);
        if (grp != 0) {
            ch = (char *)((unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c);
            txt_Format(text, 0x4205, bld);
            he_MessageBoxQuickjump(HM_I(ch, 4), HM_I(ch, 4), text, 0, 0x5fc, HM_I(bld, 1), HM_I(grp, 2), (char *)(unsigned)&ch_EmptyName, (char *)(unsigned)&ch_EmptyName);
            sc = U_I((unsigned)&d3_CurrentScene);
            d3if_SelectScene(init_ObjectSceneUniverse(grp), 1);
            {
                int o = (int)evt_Load((unsigned)&full_data_629070);
                if (o != 0) {
                    HM_I(o, 0xc8c) = 1;
                    HM_I(o, 0x104) = HM_I(grp, 2);
                    evt_RunScriptArgs((EVTSCRIPT *)o, 1, HM_I(ev, 0xac));
                }
            }
            d3if_SelectScene(sc, 1);
        }
        HM_I(ev, 0x58) = 0;
        HM_I(ev, 0x70) = 1;
        return;
    }
    if (HM_I(ev, 0x70) == 1 && !(HM_B(ev, 0x78) & 4)) {
        he_Unk5741f0(ev);
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
    }
}
