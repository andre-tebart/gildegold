/* 0x52fa10, creates the scroll parchment form "misc\scroll_perga" (0x623ea0) with its two end pieces (0x623ea4 / 0x623ea8, picture 0x6e0) in the current form; returns the form or -1 */

extern int ai_Unk623ea0, ai_Unk623ea4, ai_Unk623ea8;  /* 0x623ea0, 0x623ea4, 0x623ea8 */
int sv_Unk52fa10(void)
{
    if (ai_Unk623ea0 != -1)
        return -1;
    ai_Unk623ea0 = d2_OpenForm(0, 0, "misc\\scroll_perga");
    d2_CentreViewWindows(ai_Unk623ea0);
    if (ai_Unk623ea0 == -1)
        return -1;
    d2_SetForm(ai_Unk623ea0, 0);
    ai_Unk623ea4 = d2_WinOpenPic(0, 0, 0x6e0, d2_LastWindow);
    ai_Unk623ea8 = d2_WinOpenPic(0, 0x1d1, 0x6e0, d2_LastWindow);
    return ai_Unk623ea0;
}
