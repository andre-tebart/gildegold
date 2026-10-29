/* 0x583240, the graveyard contact window (1205 B, ROUGH SKELETON): out = the picked entry (0 / the entry, its
   id, the byte 0x4ef6b0, the second entry, its id), rec = the record, title = the window's title text, flag =
   0 / 1, cb = the callback. The window "Locations_Friedhof_Graves" (0x62a580) with 0x476f80 (0, 0) / 0x475600,
   the sound 0x52ea30, the texts 0x62a578 ("%C Z %s", title) and the sections 2 / 3 / 6 / 4 / 5 / 7 (0x6ed938 is
   kept per section); then the buttons 0x582730 (rec, w), the list 0x5828c0 (rec, w, -1) and the detail
   0x582b40 (rec, w, -1, flag); 0xf8a668 = -1 and the pump loop 0x5309a0 (this window, 0x677c7): the clicks on
   the four buttons (w + 0x??, the selection 0x170), the entries (0x6c) and the detail (param_4 = 1, w + 0x68)
   are dispatched here: a button sets the selection and redraws (0x82730 / 0x828c0 / 0x82b40 again, the second
   from the entry index), an entry fills out (the entry / its id / 0x4ef6b0 / the second entry) and with no
   callback sets 0x6f22a0 = 1 or calls cb (rec, &the entry) between 0x475590 (the window, 0) / (1); the timer
   0x6333cc + 0x96 redraws the three parts each tick; the escape 0x14673a0 -> 0x6f22a0 = 1 */

void d2_SetViewEnabled(int view, int on);

void ai_Unk583240(int *out, char *rec, char *title, int flag, void (*cb)())
{
    int w[0x50];
    int win;
    int sel;
    int entry;
    int redraw;
    int tick;
    int i;

    if (out) {
        out[0] = 0;
        out[3] = 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a580);
    memset(w, 0, sizeof(w));
    w[0] = win;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText((char *)(unsigned)&full_data_62a578, title);
    d2_SetForm(win, 2);
    d2_SetForm(win, 3);
    d2_SetForm(win, 6);
    d2_SetForm(win, 4);
    d2_SetForm(win, 5);
    d2_SetForm(win, 7);
    sel = 0;
    entry = 0;
    ai_Unk582730(rec, w);
    ai_Unk5828c0(rec, w, -1);
    ai_Unk582b40(rec, w, -1, flag);
    U_I((unsigned)&d2_ClickedId) = -1;
    /* ROUGH: the click dispatch (the four buttons, the 16 entries and the detail) and the redraw flags are not
       written yet; the window list w matches local_1a0, its selection w[0x170] / entry w[0x6c] the locals */
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk583240, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
        tick = U_I((unsigned)&ai_Unk6333cc);
        (void)tick;
        (void)sel;
        (void)entry;
        (void)cb;
        (void)i;
        (void)redraw;
    }
}
