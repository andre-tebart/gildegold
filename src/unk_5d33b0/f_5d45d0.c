/* 0x5d45d0, the save window (839 B): the twin of ai_LoadGameWindow (the same form "menu\loadgame_new",
   the view placement, the fitted form, the pointer sprite, the caption 0x1c58, the slider 0x214 x 0x168
   of the list control, ai_FillSaveSlots (win, 1, "gamedata/saves", mode 0)), but the two autosave /
   quicksave slots (0 / 1) are disabled (d2_EnableObject 0) and only the slots 2 .. 15 are clickable.
   The pump sv_RunFrame runs with this function and the frame's mask; a mouse event asks the screen to
   pop and, when dm_L.lclick is set and the list is not scrolling (the window's scrollRemY is 0), the
   slots 2 .. 15 are walked with the hit test of ai_LoadGameWindow. A hit disables the slot object and
   opens the name editor (ai_EditText) at the caption's position on the slot's title; a name that is
   not cancelled (ai_MessageBox 0x101 asks first when "Gamedata\Saves\<name>.SAV" already exists) is
   saved: the file name "Gamedata/saves/GILDE_SAVEGAME_<i>.SAV", the screen image image (0x9600 bytes)
   into ai_ScreenImage, ls_SaveSlot = i, the message box 0x14414e8, ls_SaveGame (name, title, 1), the
   pop request. The slot object is enabled again. Returns 0.
   @deep body written from tools/disasm 0x5d45d0 and the meaning matches (the twin setup with the caption 0x1c58, ai_FillSaveSlots gamedata/saves mode 0, the two disabled autosave/quicksave slots, the list control d2_GetViewWin(win, 1) and its scrollRemY test, the slot walk 2 .. 15, the hit test, the name editor ai_EditText on the slot title, the overwrite question 0x14414ec, the file GILDE_SAVEGAME_%i.SAV, the 0x9600-byte image copy, ls_SaveSlot, sv_OpenMessageBox 0x14414e8, ls_SaveGame, the pop request, the re-enable). The 19 remaining code hunks have ONE root: the original splits the pending argument cleanup of the d2_GetViewWin call as add esp,8 right after it (and then add esp,0x40 / add esp,0x18 later), ours cleans the same 24 dwords as add esp,0x40 / add esp,0x20, which shifts every later [esp+..] by 8 - VC6's deferred push cleanup / argument-slot reuse (the same split as ai_LoadGameWindow / ai_JoinGameWindow, which grade behaviour). Tried: the exists = 0 + if(vfs_FindFile(...)) exists = 1 spelling (a real gain, 36 -> 21 hunks), the exists = 0 placement before the sprintf (21 -> 19), the path/name buffer sizes to make the frame 0x6628 exactly, the 2-argument d2_GetViewWin prototype + a cast for the six-argument calls (byte-neutral, Q7 fixed); the push 0x101 / push ecx order is the cdecl argument order and is not chased. spell.py: no menu rule for the cleanup split */

/* ---- the two save / load window bodies and the three option screen stubs are defined below
   0x5d7770 (out of address order: 0x5d45d0 / 0x5d4920 / 0x5d5fd0 / 0x5d6580 / 0x5d6950 belong before
   it). VC6 drops a call whose result is unused to a function whose side-effect-free body it has
   already seen (an empty stub, or a `return 0;` stub); the original's caller 0x5d7770 was compiled
   against the real bodies. Move them back into address order once they have their bodies. ---- */

int ai_SaveOptionWindow(int image)
{
    unsigned mask;
    int win;
    int obj;
    int x20, x1c, x24;
    int exists;
    VFSDIR *found;
    int i;
    char path[0x104];
    char name[0x104];
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];
    int view;

    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindowF(win, 2);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(0, 2);
    io_PrintText(0x1c58);
    d2_SetForm(0, 0);
    obj = ((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &x20, &x1c,
                                                                     &x24); /* @original: the original pushes six
        arguments (win, 1, 0x82, &x20, &x1c, &x24) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    ((int (*)(int, int, int, int))sv_CreateFormSlider)(0x214, 0x168, d2_LastWindow, obj); /* @original:
        the original pushes four of the eight arguments of sv.c's slider builder */
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/saves", 0);
    d2_EnableObject(slots[0].obj, 0);
    d2_EnableObject(slots[1].obj, 0);
    obj = d2_GetViewWin(win, 1);
    while (sv_RunFrame((int)ai_SaveOptionWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0 && d2_Wins[obj].scrollRemY == 0) {
            for (i = 2; i < 0x10; i++) {
                if (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                    || (d2_HitWindow != -1
                        && d2_ObjsV[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win])) {
                    exists = 0;
                    sprintf(path, "Gamedata\\Saves\\%s.SAV", slots[i].name);
                    if (vfs_FindFile(path, vfs_Root, &found) != 0)
                        exists = 1;
                    d2_EnableObject(slots[i].obj, 0);
                    if (ai_EditText(d2_ObjsV[slots[i].obj].x, d2_ObjsV[slots[i].obj].y,
                                    slots[i].title) == 0) {
                        d2_EnableObject(slots[i].obj, 1);
                        break;
                    }
                    if (exists == 0 || ai_MessageBox(0x101, ai_SaveOverwriteText) != 0) {
                        sprintf(name, "Gamedata/saves/GILDE_SAVEGAME_%i.SAV", i);
                        memcpy(ai_ScreenImage, (void *)image, 0x9600);
                        ls_SaveSlot = i;
                        view = sv_OpenMessageBox(ai_SaveProgressText, 0);
                        ls_SaveGame(name, slots[i].title, 1);
                        d2_CloseView(view);
                        sv_PopRequest = 1;
                    }
                    d2_EnableObject(slots[i].obj, 1);
                }
            }
        }
    }
    d2_CloseView(win);
    return 0;
}
