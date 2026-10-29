/* 0x5c0970, the tabbed special-amount list window ("special_amt3" 0x62d214, 1984 B, ROUGH SKELETON): the
   eight tab entries 0x62d14c + i * 0x14 ([0] the count, [1] the tab control, [2] the fill function of the
   page, [3] the row builder), cleared by 0x5c08b0, the tab controls 0x46bf10 / 0x46c260, the list records
   (0xe dwords), the 0x5c08d0 slider refresh, the empty page 0x5bfed0, the click dispatch 0x14673a8 /
   0xf8a668 / 0x5fe710 and the pump 0x5309a0 (this, 0x657c7); 0x4758f0 (win) at the end. The body is left
   as a comment (the Ghidra call arguments of the function pointer calls are scrambled) */

int ai_Unk5c0970(int param_1, int param_2, unsigned int param_3, int param_4)
{
    int win;
    int r;

    r = -1;
    if ((int)param_3 >= 0 && (int)param_3 < 8)
        U_I((unsigned)&full_data_6f2f8c) = param_3;
    if (U_I((unsigned)&full_data_6f2f8c) > 7)
        U_I((unsigned)&full_data_6f2f8c) = 0;
    ai_Unk5c08b0();
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d214);
    d2_CentreViewWindows(win);
    /* ROUGH SKELETON: the tab table fill through the function pointers 0x62d154 (page fill) and 0x62d158
       (row add), the tab controls 0x46bf10 (0xbf) with 0x46c260 (0xb9) of the screen 0x143fbe0, the page
       switch 0x5bfed0 / 0x5c08d0, the click dispatch and the callback param_4; the loop ends on the pump */
    while (sv_RunFrame(ai_Unk5c0970, 0x657c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    return r;
}
