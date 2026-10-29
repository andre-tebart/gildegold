/* 0x5d4920, the save window of a network game (836 B): the twin of ai_SaveOptionWindow (the same form
   "menu\loadgame_new", the view placement, the fitted form, the pointer sprite, the caption 0x1c58, the
   slider 0x214 x 0x168 of the list control, the one disabled autosave slot 0, the list control and its
   scrollRemY test, the slot walk 2 .. 15 with the same hit test, the name editor ai_EditText on the
   slot's title) but the save is the network request: "Gamedata\network\<name>.SAV" is looked up first
   (vfs_FindFile) and ai_MessageBox 0x101 asks when it exists; a saveable player
   (ai_HasQuicksavePlayer) sends the 0xf request with the CMQUICKSAVE record (slot i, the title as the
   name) through cm_SendExSysMessage, else the status text 0x14414a4 says there is no player to save.
   The slot object is enabled again. Returns 0.
   @deep body written from tools/disasm 0x5d4920 (the twin of ai_SaveOptionWindow with ai_FillSaveSlots gamedata/network mode 1, the one disabled slot 0, the list control + scrollRemY test, the slot walk 2 .. 15 and hit test, ai_EditText on the title, the exists check of Gamedata\network\<name>.SAV, the overwrite box 0x14414ec, ai_HasQuicksavePlayer -> sv_SetStatusText 0x14414a4 else the CMQUICKSAVE 0xf request through cm_SendExSysMessage, sv_PopRequest = 1, the re-enable) and the meaning matches. The 17 remaining code hunks have ONE root: the original splits the pending argument cleanup as add esp,8 right after the d2_GetViewWin call (then add esp,0x40 / add esp,0x10), ours cleans the same 32 dwords as add esp,0x40 / add esp,0x18, shifting every later [esp+..] by 8 - VC6's deferred push cleanup / argument-slot reuse (the same split in ai_LoadGameWindow / ai_JoinGameWindow, which grade behaviour). Tried: the sv_PopRequest = 1 in the save path (a real gain), the image parameter named, the path/rec buffer sizes to make the frame 0x65a0 exact, the 2-argument d2_GetViewWin prototype + a cast for the six-argument call (Q7). push 0x101 before push ecx is the cdecl evaluation order and is not chased. spell.py: no menu rule for the cleanup split */

int ai_SaveNameWindow(int image)
{
    unsigned mask;
    int win;
    int obj;
    int x20, x1c, x24;
    int exists;
    VFSDIR *found;
    int i;
    char path[0x104];
    CMQUICKSAVE rec;
    SAVESLOTVIEW slots[0x10];
    char list[0x4200];

    (void)image;
    mask = sv_FrameMask;
    win = d2_OpenForm(0, 0, "menu\\loadgame_new");
    d2_PlaceViewWindows(win, 1);
    d2_PlaceMainWindow(win, 2);
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
    ai_FillSaveSlots(win, 1, slots, list, "gamedata/network", 1);
    d2_EnableObject(slots[0].obj, 0);
    obj = d2_GetViewWin(win, 1);
    while (sv_RunFrame((int)ai_SaveNameWindow, mask) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (dm_L.lclick != 0 && d2_Wins[obj].scrollRemY == 0) {
            for (i = 2; i < 0x10; i++) {
                if (d2_PointerObj == slots[i].obj || d2_HitWindow == slots[i].win
                    || (d2_HitWindow != -1
                        && d2_Objs[d2_Wins[d2_HitWindow].parent].win == &d2_Wins[slots[i].win])) {
                    exists = 0;
                    crt_Sprintf(path, "Gamedata\\network\\%s.SAV", slots[i].name);
                    if (vfs_FindFile(path, vfs_Root, &found) != 0)
                        exists = 1;
                    d2_EnableObject(slots[i].obj, 0);
                    if (ai_EditText(d2_Objs[slots[i].obj].x, d2_Objs[slots[i].obj].y,
                                    slots[i].title) == 0) {
                        d2_EnableObject(slots[i].obj, 1);
                        break;
                    }
                    if (exists == 0 || ai_MessageBox(0x101, ai_SaveOverwriteText) != 0) {
                        if (ai_HasQuicksavePlayer() == 0) {
                            sv_SetStatusText(ai_QuickSaveNoPlayerText);
                        } else {
                            memset(&rec, 0, sizeof rec);
                            rec.slot = i;
                            strcpy(rec.name, slots[i].title);
                            cm_SendExSysMessage(0xf, &rec);
                        }
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
