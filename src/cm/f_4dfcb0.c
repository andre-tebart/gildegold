/* 0x4dfcb0, the question "accept the duel?" (text 0x10de, "cutscenes\\duell", a box with the two answers): m + 0x94 = 1 for the first answer, 0 for the second */

void ai_Unk4dfcb0(char *m)
{
    int v, dlg;

    v = d2_OpenForm(0, 0, "cutscenes\\duell");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    io_PrintText(0x10de, *(int *)ai_Unk6f0d74);
    m[0x94] = 0;
    m[0x14] = ch_EmptyName;
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtr + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, ai_Unk60d364 * 0x4b, 1);
    while (sv_RunFrame(ai_Unk4dfcb0, ai_Unk60d360) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            m[0x94] = 1;
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            m[0x94] = 0;
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(v);
}
