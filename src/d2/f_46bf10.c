/* 0x46bf10, creates a slider in a dialog (position relative to the dialog) */

int d2_WinCreateSlider(int x, int y, int kind, int unk, int dlg)
{
    int id;

    id = d2_CreateSlider(d2_Wins[dlg].x + x, d2_Wins[dlg].y + y, kind, unk);
    d2_AddToWin(id, dlg);
    return id;
}
