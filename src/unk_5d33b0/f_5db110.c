/* 0x5db110, the second poker window "Locations\\zigeuner\\poker2" (the poker hands table, opened by the fifth button of the poker
   table 0x5da030 with its view disabled; 2806 B of straight-line picture rows). Opens the form (-1: nothing), clears
   the pointer sprite, centres the view and shows the title text 0x423e; the form 3 gets a button row of one (the text
   0x4249) whose id ends the window; the form 4 gets a slider (sv_CreateFormSlider, 0x35) in the form 2 window. In the form 2 the
   texts 0x4247 / 0x4248 are printed and the table of the eight poker hands (the texts 0x423f .. 0x4246, the hand names
   "Paar" .. "Royal Flush") is built straight-line: row i at y = 0xa0 + 0x6d * i has five card pictures 0x20 apart (x = 5,
   0x25, 0x45, 0x65, 0x85; the pictures 0x775 + n: the ones that make the hand are highlighted, the others greyed) and the
   hand's name as a centred text label at x = 0xcb, 0x29 below the row (font 0x54, as wide as the window less 0xd2). The
   window's content height is raised by 8, then the pump (this function, 0x777c7) waits for the right button or the button
   and closes the view. Void. Reasoned: the rows are read off the pictures and texts (the hand rows follow the text order
   0x423f .. 0x4246); which picture number is which card is not decoded. */

void ai_PokerWindow2(void)
{
    int ids[1];
    int a, b, c;
    int form, win, o;
    int w;

    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\poker2");
    if (form == -1)
        return;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 1);
    io_PrintText((char *)0x423e);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 1, txt_Ptrs[0x4249]);
    d2_SetForm(form, 2);
    win = d2_LastWindow;
    w = d2_Wins[d2_LastWindow].w - 0xd2;
    d2_SetForm(form, 4);
    sv_CreateFormSlider(0, 0, d2_LastWindow, win, 0x35, &c, &b, &a);
    d2_SetForm(form, 2);
    io_PrintText((char *)0x4247);
    io_PrintText((char *)0x4248);
    o = d2_WinOpenPic(0x5, 0xa0, 0x779, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0xa0, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0xa0, 0x77f, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x65, 0xa0, 0x784, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x85, 0xa0, 0x78a, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0xc9, txt_Ptrs[0x423f], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x10d, 0x779, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x10d, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x10d, 0x785, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x10d, 0x786, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x10d, 0x78a, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x136, txt_Ptrs[0x4240], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x17a, 0x781, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x17a, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x17a, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x17a, 0x777, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_WinOpenPic(0x85, 0x17a, 0x77b, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x1a3, txt_Ptrs[0x4241], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x1e7, 0x775, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x1e7, 0x77a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x1e7, 0x77f, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x1e7, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x1e7, 0x786, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x210, txt_Ptrs[0x4242], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x254, 0x781, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x254, 0x782, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x254, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x254, 0x789, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x254, 0x78a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x27d, txt_Ptrs[0x4243], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x2c1, 0x777, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x2c1, 0x77b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x2c1, 0x783, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x2c1, 0x787, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x2c1, 0x78b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x2ea, txt_Ptrs[0x4244], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x32e, 0x789, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x32e, 0x78a, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x32e, 0x78b, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x32e, 0x78c, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x32e, 0x77b, d2_LastWindow);
    d2_Objs[o].greyed = 1;
    o = d2_CreateWinText(0xcb, 0x357, txt_Ptrs[0x4245], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    o = d2_WinOpenPic(0x5, 0x39b, 0x778, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x25, 0x39b, 0x77c, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x45, 0x39b, 0x780, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x65, 0x39b, 0x784, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_WinOpenPic(0x85, 0x39b, 0x788, d2_LastWindow);
    d2_Objs[o].highlighted = 1;
    o = d2_CreateWinText(0xcb, 0x3c4, txt_Ptrs[0x4246], d2_LastWindow);
    d2_Objs[o].w = w;
    d2_SetObjFont(o, 0x54);
    d2_ClickedId = 1;
    d2_Wins[d2_LastWindow].contentHeight += 8;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_PointerObj == ids[0])
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PokerWindow2, 0x777c7) != 0);
    d2_CloseView(form);
}
