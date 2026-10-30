/* 0x5d6950, the game options screen "menu\options_game" 0x62db30 (1514 B): the same window family as 0x5d5fd0. Fills
   eleven controls of the view 1: four sliders (the game speed ai_OptionSpeed 0 .. 0xa0, the mouse speed 0 .. 0x1f4, the scroll
   speed 0 .. 100, the camera speed byte 0 .. 100) and seven two-state / list options from ai_GameSwitches (bytes 0, 2, 3), ai_GamePanel
   (the panel list 0 .. 4, bytes 1 and 3) and ai_OptionPanelHelp; the switch byte 0 and the panel byte 1 are shown disabled. Pumps
   the screen (sv_RunFrame, this function as the callback): a click or the right button pops it, the first button of the row applies.
   On apply the controls are read back into the option globals (switch byte 0 is cleared, its control is not read) and, when the
   panel list changed, the stored panel is updated, the view is disabled and, while a game runs, the screen is rebuilt
   (sv_DestroyScreen, sv_BuildScreenLayout of sv_PanelImage, the fade ai_LeaveGameScreen, sv_ScreenDirty = 1); then the INI is
   written (ai_SaveOptions) and the speed factor applied (ai_ApplyOptions). Void; opened by the options menu 0x5d7770 and main.c.
   Text slots (txt_Ptrs): the labels 0x1c96 .. 0x1ca1 (0x1c9f unused), the two-state items 0x1c7c / 0x1c7d, the panel items
   0x1c76 .. 0x1c7a, the buttons 0x1ca2 / 0x1ca3. */

void ai_OptionsGameScreen(void)
{
    unsigned mask;
    int form, apply;
    int ids[2];
    int okId, cancelId;
    int speed, mouse, scroll, camera, sw0, sw2, sw3, panel, panel1, help, panel3;

    apply = 0;
    mask = sv_FrameMaskU & 0xffffdfff;
    form = d2_OpenForm(0, 0, "menu\\options_game");
    sv_SetScreenCenter(mask);
    d2_PlaceViewWindows(form, 1);
    d2_PlaceMainWindowF(form, 2);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(form, 2);
    io_PrintText(0x1c59);
    d2_SetForm(form, 1);
    speed = d2_GetWinObj(form, 1, 0);
    mouse = d2_GetWinObj(form, 1, 1);
    scroll = d2_GetWinObj(form, 1, 2);
    camera = d2_GetWinObj(form, 1, 3);
    sw0 = d2_GetWinObj(form, 1, 4);
    sw2 = d2_GetWinObj(form, 1, 5);
    sw3 = d2_GetWinObj(form, 1, 6);
    panel = d2_GetWinObj(form, 1, 9);
    panel1 = d2_GetWinObj(form, 1, 10);
    help = d2_GetWinObj(form, 1, 11);
    panel3 = d2_GetWinObj(form, 1, 12);
    d2_SetObjValue4(speed, 0, 0xa0, ai_OptionSpeed);
    d2_SetSliderFlags(speed, 0x882);
    d2_SetLabel(speed, txt_Ptrs[0x1c96]);
    d2_SetObjValue4(mouse, 0, 0x1f4, ai_OptionMouseSpeed);
    d2_SetSliderFlags(mouse, 0x882);
    d2_SetLabel(mouse, txt_Ptrs[0x1c97]);
    d2_SetObjValue4(scroll, 0, 100, ai_OptionScrollSpeed);
    d2_SetSliderFlags(scroll, 0x882);
    d2_SetLabel(scroll, txt_Ptrs[0x1c98]);
    d2_SetObjValue4(camera, 0, 100, ai_OptionCameraSpeed & 0xff);
    d2_SetSliderFlags(camera, 0x882);
    d2_SetLabel(camera, txt_Ptrs[0x1c99]);
    d2_SetObjValue4(sw0, 0, 1, ai_GameSwitches.word & 0xff);
    d2_SetSliderFlags(sw0, 0x82);
    d2_SetLabel(sw0, txt_Ptrs[0x1c9a]);
    d2_SetList(sw0, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_EnableObject(sw0, 0);
    d2_SetObjValue4(sw2, 0, 1, ai_GameSwitches.byte[2]);
    d2_SetSliderFlags(sw2, 0x82);
    d2_SetLabel(sw2, txt_Ptrs[0x1c9b]);
    d2_SetList(sw2, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(sw3, 0, 1, ai_GameSwitches.byte[3]);
    d2_SetSliderFlags(sw3, 0x82);
    d2_SetLabel(sw3, txt_Ptrs[0x1c9c]);
    d2_SetList(sw3, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(panel, 0, 4, ai_GamePanel.word & 0xff);
    d2_SetSliderFlags(panel, 0x82);
    d2_SetLabel(panel, txt_Ptrs[0x1c9d]);
    d2_SetList(panel, 5, txt_Ptrs[0x1c76], txt_Ptrs[0x1c77], txt_Ptrs[0x1c78], txt_Ptrs[0x1c79], txt_Ptrs[0x1c7a]);
    d2_SetObjValue4(panel1, 0, 1, ai_GamePanel.byte[1]);
    d2_SetSliderFlags(panel1, 0x82);
    d2_SetLabel(panel1, txt_Ptrs[0x1c9e]);
    d2_SetList(panel1, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_EnableObject(panel1, 0);
    d2_SetObjValue4(help, 0, 1, ai_OptionPanelHelp.word & 0xff);
    d2_SetSliderFlags(help, 0x82);
    d2_SetLabel(help, txt_Ptrs[0x1ca1]);
    d2_SetList(help, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetObjValue4(panel3, 0, 1, ai_GamePanel.byte[3]);
    d2_SetSliderFlags(panel3, 0x82);
    d2_SetLabel(panel3, txt_Ptrs[0x1ca0]);
    d2_SetList(panel3, 2, txt_Ptrs[0x1c7c], txt_Ptrs[0x1c7d]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x1ca2], txt_Ptrs[0x1ca3]);
    okId = ids[0];
    cancelId = ids[1];
    while (sv_RunFrame((int)ai_OptionsGameScreen, mask) != 0) {
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
        ai_OptionSpeed = d2_GetObjValue(speed);
        ai_OptionCameraSpeedByte = d2_GetObjValue(camera);
        ai_OptionMouseSpeed = d2_GetObjValue(mouse);
        ai_OptionScrollSpeed = d2_GetObjValue(scroll);
        ai_GameSwitches.byte[0] = 0;
        ai_GameSwitches.byte[2] = d2_GetObjValue(sw2);
        ai_GameSwitches.byte[3] = d2_GetObjValue(sw3);
        ai_GamePanel.byte[1] = d2_GetObjValue(panel1);
        ai_GamePanel.byte[3] = d2_GetObjValue(panel3);
        ai_OptionPanelHelp.byte[0] = d2_GetObjValue(help);
        if (d2_GetObjValue(panel) != (ai_GamePanel.word & 0xff)) {
            ai_GamePanel.byte[0] = d2_GetObjValue(panel);
            d2_SetViewEnabled(form, 0);
            if (ai_GameStateB != 0) {
                sv_DestroyScreen();
                sv_BuildScreenLayout(sv_PanelImage());
                ai_LeaveGameScreen();
                sv_ScreenDirty = 1;
            }
        }
        ai_SaveOptions();
        ai_ApplyOptions();
    }
    d2_CloseView(form);
}
