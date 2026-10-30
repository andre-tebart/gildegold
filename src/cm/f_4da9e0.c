/* 0x4da9e0, the vote dialog "cutscenes\\sitzungen" (buttons from the text 0x12c6): the answer of the player's choice; *out the click */

int ai_Unk4da9e0(char *a, char *b, char *c, int *out)
{
    int v, b1, b2, b3, dlg, r;
    int tmp[1];

    *out = 0;
    v = d2_OpenForm(0, 0, "cutscenes\\sitzungen");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 1);
    d2_SetForm(v, 0);
    r = io_PrintText(0x12c6, *(unsigned short *)b, *(unsigned short *)a);
    b1 = d2_GetWinObj(v, 0, r);
    d2_GetWinObj(v, 0, r + 1);
    b3 = d2_GetWinObj(v, 0, r + 2);
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, 0x5dc, 1);
    r = ai_TrustedOfTwo(a, b, c, tmp);
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_Unk4da9e0, ai_Unk60d360) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            r = 0;
            sv_PopRequest = 1;
            if (ai_Unk5fe710 != b1)
                r = (ai_Unk5fe710 != b3) + 1;
        }
    }
    d2_CloseView(v);
    return r;
}
