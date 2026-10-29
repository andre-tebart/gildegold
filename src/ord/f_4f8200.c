/* 0x4f8200, a privilege dialog: loads the form privillegien\\prv_small (mode 0), prv_big (1) or prv_very_big, shows the text, then waits through 0x5309a0 (this function as its callback, arg its argument), setting the flag 0x6f22a0 while the redraw flag 0x14673a0 is set; closes the form */

int ord_PrivilegeDialog(char *text, int mode, int arg)
{
    int v;

    if (mode == 0)
        v = d2_OpenForm(0, 0, "privillegien\\prv_small");
    else if (mode == 1)
        v = d2_OpenForm(0, 0, "privillegien\\prv_big");
    else
        v = d2_OpenForm(0, 0, "privillegien\\prv_very_big");
    d2_CentreViewWindows(v);
    d2_SetForm(v, 1);
    io_PrintText(text);
    d2_ClickedId = -1;
    while (sv_RunFrame(ord_PrivilegeDialog, arg) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(v);
}
