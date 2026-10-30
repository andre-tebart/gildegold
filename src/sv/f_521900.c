/* 0x521900, opens a non-modal message box (the form by the flags 0x10 BIG / 0x20 VERY_BIG), shows the text, pumps
   the messages once and returns the form */

int sv_OpenMessageBox(int text, unsigned char flags)
{
    int form;

    if ((flags & 0x10) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if ((flags & 0x20) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else
        form = d2_OpenForm(0, 0, "misc\\Messagebox");
    d2_CentreViewWindows(form);
    io_PrintText(text);
    sv_RunFrame(0, sv_FrameMask);
    return form;
}
