/* 0x475f70, selects the current window by its number in a view */

int d2_SetForm(int view, int n)
{
    if (view != 0)
        d2_CurrentView = view;
    if (n <= d2_Views[d2_CurrentView].nwins && n >= 0) {
        d2_LastWindow = d2_Views[d2_CurrentView].wins[n];
        d2_LastWindowPtr = &d2_Wins[d2_Views[d2_CurrentView].wins[n]];
        return 1;
    }
    err_Message("d2_SetForm(): invalid win number");
    return 0;
}
