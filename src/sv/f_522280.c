/* 0x522280, closes the panel widgets (0x46e710, id := -1) and hides the rest */

void sv_Unk522280(void)
{
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 0);
    if (ai_Unk623820 != -1) {
        d2_RemoveObj(ai_Unk623820);
        ai_Unk623820 = -1;
    }
    if (ai_Unk623824 != -1) {
        d2_RemoveObj(ai_Unk623824);
        ai_Unk623824 = -1;
    }
    if (ai_Unk623828 != -1) {
        d2_RemoveObj(ai_Unk623828);
        ai_Unk623828 = -1;
    }
    if (ai_Unk623844 != -1) {
        d2_RemoveObj(ai_Unk623844);
        ai_Unk623844 = -1;
    }
    if (ai_Unk62382c != -1) {
        d2_RemoveObj(ai_Unk62382c);
        ai_Unk62382c = -1;
    }
    if (ai_Unk623830 != -1) {
        d2_RemoveObj(ai_Unk623830);
        ai_Unk623830 = -1;
    }
    if (ai_Unk623834 != -1) {
        d2_RemoveObj(ai_Unk623834);
        ai_Unk623834 = -1;
    }
    if (ai_Unk623838 != -1) {
        d2_RemoveObj(ai_Unk623838);
        ai_Unk623838 = -1;
    }
    if (ai_Unk62383c != -1) {
        d2_RemoveObj(ai_Unk62383c);
        ai_Unk62383c = -1;
    }
    if (ai_Unk623848 != -1) {
        d2_RemoveObj(ai_Unk623848);
        ai_Unk623848 = -1;
    }
    if (ai_Unk62384c != -1) {
        d2_RemoveObj(ai_Unk62384c);
        ai_Unk62384c = -1;
    }
    if (ai_Unk6238a4 != -1)
        d2_EnableObject(ai_Unk6238a4, 0);
}
