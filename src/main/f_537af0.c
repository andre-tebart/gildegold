/* 0x537af0, sets the loading progress bar to pct percent (the bar 0x246 wide) */

void main_SetLoadProgress(int unused, int pct)
{
    int b;

    if (U_I((unsigned)&init_StatusForm) != -1) {
        d2_SetForm(U_I((unsigned)&init_StatusForm), 0);
        d2_SetForm(U_I((unsigned)&init_StatusForm), 1);
        b = d2_GetWinObj(U_I((unsigned)&init_StatusForm), 1, 0);
        d2_SetObjValue(b, 0, 0x246, (pct * 0x246) / 100);
        d2_DrawAll();
    }
}
