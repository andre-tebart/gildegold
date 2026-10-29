/* 0x5c1500, the special-amount list window opened by the menu 0x5c1b20 (1394 B, ROUGH SKELETON): the window
   "special_amt2" 0x62d204 (0x4741f0 (0, 0, text) / 0x475600), frame 0x2ec; the body (the record rows, the
   click dispatch and the pump 0x5309a0) is left as a comment. The tail returns through 0x52d7a0 (the caller
   0x5c1b20 exits there) */

int ai_Unk5c1500(int param_1)
{
    int win;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d204);
    d2_CentreViewWindows(win);
    /* ROUGH SKELETON: the record rows of the page, the click dispatch and the pump 0x5309a0 (this, 0x657c7);
       the tail 0x4758f0 (win) */
    d2_CloseView(win);
    (void)param_1;
    return 0;
}
