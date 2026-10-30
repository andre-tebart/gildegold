/* 0x4e24c0, the player's answer to an offer m (+0x20 the price, +0x24 .. the persons in the deal, +0x44 the object, +0x48 their count, +0x4c a
   factor): when the asked character r + 0x10 is among them: the dialog "cutscenes\\pacht" (texts 0x1aa6 / 0x1aa7) with a slider between the two
   limits (0x5bd060 / 0x5085b0 of the price), r + 0x98 = 1 and r + 0x94 = the picked value when the player confirms (0xf8a668 == 0).
   ROUGH: the limits are only read from the calls */

void ai_Unk4e24c0(char *r, char *m)
{
    int i, n, id, lo, hi, v, dlg, sl, cur, t, lim[8];
    char *c;

    *(int *)(r + 0x98) = 0;
    n = *(int *)(m + 0x48);
    id = *(int *)(r + 0x10);
    for (i = 0; i < n && *(int *)(m + 0x24 + i * 4) != id; i++)
        ;
    if (i >= n || *(int *)(m + i * 4) == -1)
        return;
    *(int *)(r + 0x94) = *(int *)(m + 0x20);
    gm_BuildQuery(1, 1, *(int *)(m + 0x44));
    c = (char *)gm_FindChar(*(int *)(r + 0x10));
    if (c == 0)
        return;
    cur = *(int *)(m + 0x20);
    hi = gm_GetGoodsValue(c);
    ai_Unk5bd060(cur, hi, 0x20, lim);
    lo = gm_ConvertMoney(cur, ai_OwnPlayer);
    sl = gm_ConvertMoney(lim[0], ai_OwnPlayer);
    if (hi < cur)
        return;
    v = d2_OpenForm("cutscenes\\pacht", 0, 0);
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_ClickedId = -1;
    d2_SetForm(v, 0);
    t = io_PrintText(0x1aa6, ai_OwnPlayer);
    t = io_PrintText(0x1aa7);
    dlg = d2_GetWinObj(v, 0, t);
    d2_SetObjValue(dlg, lo, sl, lo);
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, 0x5dc, 1);
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_Unk60d360, ai_Unk4e24c0) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            sv_PopRequest = 1;
            if (d2_ClickedId != 1 && d2_ClickedId == 0) {
                *(int *)(r + 0x98) = 1;
                *(int *)(r + 0x94) = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(dlg, ai_OwnPlayer));
            }
        }
    }
    d2_CloseView(v);
}
