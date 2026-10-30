/* 0x5d5fd0, the graphics options screen "menu\options_gfx" 0x62daf8 (1454 B): opens the form, fills its nine controls
   of the view 1 (0 the resolution list, 1 .. 3 the detail / shadow / texture lists, 4 .. 6 the floor options, 7 the fog
   slider (shown as 100 - the stored fog), 8 the last list) from the option bytes (ai_GfxResolution, ai_GfxDetail,
   ai_GfxFloor, ai_GfxFog) with their labels and item lists, disables the resolution control while a game runs, and pumps the
   screen (sv_RunFrame, this function as the callback): a click or the right button pops it, the first button of the row
   (d2_PointerObj == ids[0]) applies. On apply it reads the controls back into the option bytes and writes the INI
   (ai_SaveOptions) and applies the gfx options (ai_ApplyGfxOptions); a changed resolution is stored, saved, flags the
   game for a restart (ai_LeaveGame 0, ai_GameOver 1, main_ScreenRestart 1) and reloads the options (ai_LoadOptions). Void; opened by the
   options menu 0x5d7770 and main.c.
   Text slots (txt_Ptrs 0x1c67 ..): the labels of the nine controls 0x1c67 .. 0x1c6f, the item lists 0x1c70 .. 0x1c75 and
   0x1c7c / 0x1c7d, the two buttons 0x1ca2 / 0x1ca3 (read off the original's loads, not named by the text files).
   The resolution default reads ids[1] before sv_ButtonRow has filled it (the original does the same). @deep body reconstructed from the disassembly (nine controls of view 1, ids[2] row, ok/cancel copies, apply read-back in the original order, resolution change path); 449 = 449 instructions, ONE hunk of 6 differing instructions left at 0x5d60c5: the original stores the obj8 d2_GetWinObj result at once (mov [esp+0x50],eax) and reads the dd_Has1024x768 flag into al (mov al,[flag]; add esp,0x30; test al,al; mov eax,1), ours keeps eax live, reads the flag into cl and stores obj8 after the cleanup (mov [esp+0x20],eax). Tried: declaration order of obj7/obj8, ids[2] vs ids[4] (ids[4] fixes the frame 0x30 -> 0x38), mode from a separate uninit local vs ids[1] (the original reads the uninitialised slot of the second id), 'if (flag == 0)' forms. Allocation / scheduling only. */

void ai_OptionsGfxScreen(void)
{
    unsigned mask;
    int form, apply, mode;
    int res, obj1, obj2, obj3, obj4, obj5, obj6, obj8, obj7;
    int ids[4];
    int okId, cancelId;
    int dflt;

    apply = 0;
    mask = sv_FrameMaskU & 0xffffdfff;
    form = d2_OpenForm(0, 0, "menu\\options_gfx");
    sv_SetScreenCenter(mask);
    d2_PlaceViewWindows(form, 1);
    d2_PlaceMainWindowF(form, 2);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 2);
    io_PrintText(0x1c5a);
    d2_SetForm(form, 1);
    res = d2_GetWinObj(form, 1, 0);
    obj1 = d2_GetWinObj(form, 1, 1);
    obj2 = d2_GetWinObj(form, 1, 2);
    obj3 = d2_GetWinObj(form, 1, 3);
    obj4 = d2_GetWinObj(form, 1, 4);
    obj5 = d2_GetWinObj(form, 1, 5);
    obj6 = d2_GetWinObj(form, 1, 6);
    obj7 = d2_GetWinObj(form, 1, 7);
    obj8 = d2_GetWinObj(form, 1, 8);
    mode = 1;
    if (dd_Has1024x768 == 0)
        mode = dflt;
    if (dd_Has1152x864 != 0)
        mode = 2;
    d2_SetObjValue4(res, 0, mode, ai_GfxResolution.word & 0xff);
    d2_SetSliderFlags(res, 0x82);
    d2_SetLabel(res, txt_Ptrs[0x1c67]);
    d2_SetList(res, 3, txt_Ptrs[0x1c73], txt_Ptrs[0x1c74], txt_Ptrs[0x1c75]);
    d2_SetObjValue4(obj1, 0, 2, ai_GfxDetail.word & 0xff);
    d2_SetSliderFlags(obj1, 0x82);
    d2_SetLabel(obj1, txt_Ptrs[0x1c68]);
    d2_SetList(obj1, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj2, 0, 2, ai_GfxDetail.byte[1]);
    d2_SetSliderFlags(obj2, 0x82);
    d2_SetLabel(obj2, txt_Ptrs[0x1c69]);
    d2_SetList(obj2, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj3, 0, 2, ai_GfxFloor.byte[2]);
    d2_SetSliderFlags(obj3, 0x82);
    d2_SetLabel(obj3, txt_Ptrs[0x1c6a]);
    d2_SetList(obj3, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj4, 0, 1, ai_GfxFloor.word & 0xff);
    d2_SetSliderFlags(obj4, 0x82);
    d2_SetLabel(obj4, txt_Ptrs[0x1c6b]);
    d2_SetList(obj4, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(obj5, 0, 1, ai_GfxDetail.byte[2]);
    d2_SetSliderFlags(obj5, 0x82);
    d2_SetLabel(obj5, txt_Ptrs[0x1c6c]);
    d2_SetList(obj5, 2, txt_Ptrs[0x1c70], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj6, 0, 2, ai_GfxDetail.byte[3]);
    d2_SetSliderFlags(obj6, 0x82);
    d2_SetLabel(obj6, txt_Ptrs[0x1c6d]);
    d2_SetList(obj6, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    d2_SetObjValue4(obj7, 0x32, 100, 100 - (ai_GfxFog.word & 0xff));
    d2_SetSliderFlags(obj7, 0x882);
    d2_SetLabel(obj7, txt_Ptrs[0x1c6e]);
    d2_SetObjValue4(obj8, 0, 2, ai_GfxFloor.byte[1]);
    d2_SetSliderFlags(obj8, 0x82);
    d2_SetLabel(obj8, txt_Ptrs[0x1c6f]);
    d2_SetList(obj8, 3, txt_Ptrs[0x1c70], txt_Ptrs[0x1c71], txt_Ptrs[0x1c72]);
    if (ai_GameStateB != 0)
        d2_EnableObject(res, 0);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x1ca2], txt_Ptrs[0x1ca3]);
    okId = ids[0];
    cancelId = ids[1];
    while (sv_RunFrame((int)ai_OptionsGfxScreen, mask) != 0) {
        if (dm_L.rup != 0 || kb_Last == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == okId) {
                apply = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancelId)
                sv_PopRequest = 1;
        }
    }
    if (apply != 0) {
        ai_GfxDetail.byte[0] = d2_GetObjValue(obj1);
        ai_GfxDetail.byte[1] = d2_GetObjValue(obj2);
        ai_GfxDetail.byte[2] = d2_GetObjValue(obj5);
        ai_GfxDetail.byte[3] = d2_GetObjValue(obj6);
        ai_GfxFloor.byte[0] = d2_GetObjValue(obj4);
        ai_GfxFloor.byte[1] = d2_GetObjValue(obj8);
        ai_GfxFloor.byte[2] = d2_GetObjValue(obj3);
        ai_GfxFog.byte[0] = 100 - d2_GetObjValue(obj7);
        ai_SaveOptions();
        ai_ApplyGfxOptions();
    }
    if (d2_GetObjValue(res) != (ai_GfxResolution.word & 0xff) && apply != 0) {
        ai_GfxResolution.byte[0] = d2_GetObjValue(res);
        ai_SaveOptions();
        ai_LeaveGame = 0;
        ai_GameOver = 1;
        main_ScreenRestart = 1;
        ai_LoadOptions();
    }
    d2_CloseView(form);
}
