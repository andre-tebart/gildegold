/* 0x4dfdd0, the choice of the challenged person (text 0x10e6, three answers: m + 0x94 = 2 / 3 / 4); the player's character is the
   one of m + 0x34 / m + 0x38 that is not the first */

void ai_Unk4dfdd0(char *m)
{
    char *a, *b, *w;
    int v, b1, b2, b3, dlg;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    w = b;
    if (*(int *)(a + 4) != *(int *)(ai_Unk6f0d74 + 4))
        w = a;
    v = d2_OpenForm(0, 0, "cutscenes\\duell");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    {
        int t = io_PrintText(0x10e6, ai_Unk6f0d7c, *(unsigned short *)ai_Unk6f0d74, *(unsigned short *)w);

        b1 = d2_GetWinObj(v, 0, t);
        b2 = d2_GetWinObj(v, 0, t + 1);
        b3 = d2_GetWinObj(v, 0, t + 2);
    }
    m[0x94] = 4;
    m[0x14] = ch_EmptyName;
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtr + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, ai_Unk60d364 * 0x4b, 1);
    while (sv_RunFrame(ai_Unk4dfdd0, ai_Unk60d360) != 0) {
        ai_Unk4cee20();
        if (d2_ClickedId != -1) {
            if (ai_Unk5fe710 == b1)
                m[0x94] = 2;
            else if (ai_Unk5fe710 == b2)
                m[0x94] = 3;
            else if (ai_Unk5fe710 == b3)
                m[0x94] = 4;
            sv_PopRequest = 1;
        }
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(v);
}
