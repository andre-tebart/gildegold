/* 0x5d42b0, the window that picks a saved network game to join (520 B): the twin of ai_LoadGameWindow
   (the same form "menu\loadgame_new", the view placement, the fitted form, the pointer sprite, the
   caption 0x1c57, the slider 0x214 x 0x168 of the list control d2_GetViewWin (win, 1, 0x82)) with
   ai_FillSaveSlots (win, 1, "gamedata/network", mode 1). The pump sv_RunFrame runs with this function
   and the frame's mask; a mouse event asks the screen to pop, and when dm_L.lclick is set the 16 slots
   are walked (the same hit test as ai_LoadGameWindow: the caption object, the slot's window, or a
   control of it). A hit asks for confirmation (ai_MessageBox 0x101) while a game runs, then copies the
   slot's name into ls_SavePath (the caller opens "gamedata/network/<name>.SAV"), asks the screen to
   pop and returns 1. Returns 0 otherwise. */

int ai_JoinGameWindow(void)
{
    int ret;
    unsigned mask;
    int win;
    int ctrl;
    int x14, x18, x1c;
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int i;

    ret = 0;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindow(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c57);
    d2_SetForm(0, 0);
    ctrl = ((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &x1c, &x14,
                                                                      &x18); /* @original: the original pushes six
        arguments (win, 1, 0x82, &x1c, &x14, &x18) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    ((int (*)(int, int, int, int))sv_CreateFormSlider)(0x214, 0x168, d2_LastWindow, ctrl); /* @original:
        the original pushes four of the eight arguments of sv.c's slider builder */
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/network", 1);
    while (sv_RunFrame((int)ai_JoinGameWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0) {
            for (i = 0; i < 0x10; i++) {
                if (slots[i].used != 0 && slots[i].win != -1
                    && (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                        || (d2_HitWindow != -1
                            && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win]))) {
                    if (ai_GameState == 0 || ai_MessageBox(0x101, ai_LoadGameConfirmText) != 0) {
                        strcpy(ls_SavePath, slots[i].name);
                        sv_PopRequest = 1;
                        ret = 1;
                    }
                    break;
                }
            }
        }
    }
    d2_CloseView(win);
    return ret;
}
