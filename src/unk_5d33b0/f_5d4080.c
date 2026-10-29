/* 0x5d4080, the load-game window (558 B): opens the form "menu\loadgame_new" (d2_OpenForm), places
   its views (d2_PlaceViewWindows 1 / d2_PlaceMainWindow 2), fits the form (sv_FitForm), sets the
   pointer sprite (d2_SetPointerSprite 0, 0), prints the caption 0x1c57 and builds the slider
   (sv_CreateFormSlider 0x214 x 0x168) of the list control d2_GetViewWin (win, 1, 0x82); then
   ai_FillSaveSlots (win, 1) fills the 16 slots from "gamedata/saves". The pump sv_RunFrame
   (0x5309a0) runs with this function and the mask of the frame; per frame a mouse event asks the
   screen to pop and, when dm_L.lclick is set, the 16 slots are walked: the slot is hit when the click
   is on its caption object (d2_PointerObj), in its window (d2_HitWindow) or in a control of it (the
   parent object of the clicked window belongs to the slot's window). A hit asks for confirmation
   (ai_MessageBox 0x101) while a game runs, then writes "Gamedata\Saves\<name>.SAV" into ls_SavePath,
   sets the load mode (ai_GameFlags 10) and the pop request and returns 1. Returns 0 otherwise. */

int ai_LoadGameWindow(void)
{
    int ret;
    unsigned mask;
    int win;
    int ctrl;
    int x14, x18, x1c;
    char path[0x104];
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
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/saves", 0); /* @original: the original call pushes
        a sixth argument 0 that 0x5d3ab0 never reads */
    while (sv_RunFrame((int)ai_LoadGameWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0) {
            for (i = 0; i < 0x10; i++) {
                if (slots[i].used != 0 && slots[i].win != -1
                    && (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                        || (d2_HitWindow != -1
                            && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win]))) {
                    if (ai_GameState == 0 || ai_MessageBox(0x101, ai_LoadGameConfirmText) != 0) {
                        sprintf(path, "Gamedata\\Saves\\%s.SAV", slots[i].name);
                        strcpy(ls_SavePath, path);
                        ai_GameFlagsWord = 10;
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
