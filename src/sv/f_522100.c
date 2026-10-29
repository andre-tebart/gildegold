/* 0x522100, enables (n != 0) or disables (n == 0) the panel widgets (0x475aa0 marks an object disabled
   when its on argument is 0): the flag 0x623e88 = n, and every widget id of the panel set (0x623860 by
   0x475590, 0x53dad0 for the list, the others by 0x475aa0, the strip 0x623840 through
   its record of 0xf8f128, the table 0x7662e0) that is not -1 */

void sv_SetPanelEnabled(int n)
{
    int *q;

    ai_Unk623e88 = n;
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, n);
    he_Unk53dad0(n);
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, n);
    if (ai_Unk623820 != -1)
        d2_EnableObject(ai_Unk623820, n);
    if (ai_Unk623824 != -1)
        d2_EnableObject(ai_Unk623824, n);
    if (ai_Unk623828 != -1)
        d2_EnableObject(ai_Unk623828, n);
    if (ai_Unk62382c != -1)
        d2_EnableObject(ai_Unk62382c, n);
    if (ai_Unk623830 != -1)
        d2_EnableObject(ai_Unk623830, n);
    if (ai_Unk623834 != -1)
        d2_EnableObject(ai_Unk623834, n);
    if (ai_Unk623844 != -1)
        d2_EnableObject(ai_Unk623844, n);
    if (ai_Unk623838 != -1)
        d2_EnableObject(ai_Unk623838, n);
    if (ai_Unk62383c != -1)
        d2_EnableObject(ai_Unk62383c, n);
    if (ai_Unk6238a4 != -1 && n == 0)
        d2_EnableObject(ai_Unk6238a4, 0);
    if (ai_Unk623848 != -1)
        d2_EnableObject(ai_Unk623848, n);
    if (ai_Unk62384c != -1)
        d2_EnableObject(ai_Unk62384c, n);
    if (ai_Unk623840 != -1)
        d2_EnableObject(*(int *)(ai_Unkf8f128 + 0x264 + ai_Unk623840 * 0x3c4), n);
    q = ai_Unk7662e0;
    do {
        if (*q != -1)
            d2_EnableObject(*q, n);
        q++;
    } while ((int)q < (int)((unsigned)&ai_Unk7662e0 + 0x3c));
}
