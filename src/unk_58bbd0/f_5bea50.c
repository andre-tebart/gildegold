/* 0x5bea50, the special-amount list window ("special_amt2" 0x62d204, 1648 B, ROUGH SKELETON): the caption
   pages 0x475f70 (2 / 3 / 1) with 0x515960 (the kind names 0x62a9a8 / the character name), the record
   array (8 records of 0xe dwords), the fills 0x4e70f0 (kind 7) / 0x4e6fc0 (kinds 1 .. 6 and 8), the
   per-record init of 0x5be1d0 (the texts 0x6ca .. 0x6d2), the row builders 0x5bd5e0 (kind 0 / 7) /
   0x5bdb60 (the others), the click dispatch 0x1467398 / 0xf8a668 / 0x5fe710 with the callback param_5 and
   the pump 0x5309a0 (this, 0x657c7); 0x4758f0 (win) at the end. The body is left as a comment */

int ai_Unk5bea50(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    int win;
    int r;

    r = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d204);
    d2_CentreViewWindows(win);
    /* ROUGH SKELETON: the captions, the record fill and the row builders listed above; the loop ends on
       the pump and the click / the callback decides r */
    while (sv_RunFrame(ai_Unk5bea50, 0x657c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    return r;
}
