/* 0x476b70, clears the pending input state */

void d2_ClearInput(void)
{
    d2_ClickedId = -1;
    di_ClearButtons();
    kb_Last = 0;
}
