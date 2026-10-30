/* 0x4dab50, the dialog of one candidate vote: up to 4 candidates of the list c (ids), returns the id picked in *out / the result */

int ai_Unk4dab50(int a, char *b, int *c, int *out)
{
    int hdr[2], kind[2], cand[4], btn[4], rec[8];
    int n, i, r, v, dlg, res, k;
    unsigned int f;
    int tmp[1];

    *out = 0;
    if (amt_GetOfficeI(a, hdr) == 0)
        return -1;
    f = (unsigned char)hdr[2];
    if (amt_GetKind(f, rec) == 0)
        return -1;
    k = amt_GetOfficeApplicants(f, 2, cand);
    n = 0;
    for (i = 0; i < 4; i++) {
        r = gm_FindChar(c[i]);
        if (r != 0)
            cand[n++] = r;
    }
    if (n == 0)
        return -1;
    res = ai_TrustedOfFour(a, b, c, tmp);
    v = d2_OpenForm(0, 0, "cutscenes\\sitzungen");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 1);
    d2_SetForm(v, 0);
    if (k < 2)
        io_PrintText(0x12dc, *(unsigned short *)b, *(unsigned short *)b, f + 0x253);
    else
        io_PrintText(0x12dd, *(unsigned short *)b, *(unsigned short *)b, f + 0x253);
    for (i = 0; i < n; i++)
        btn[i] = d2_GetWinObj(v, 0, io_PrintText("$a%1N3$A", *(unsigned short *)cand[i]));
    for (; i < 4; i++)
        btn[i] = -1;
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, 0x5dc, 1);
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_Unk4da9e0, ai_Unk60d360) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            for (i = 0; i < n; i++) {
                if (ai_Unk5fe710 == btn[i]) {
                    res = *(int *)(cand[i] + 4);
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
    }
    d2_CloseView(v);
    return res;
}
