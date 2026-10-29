/* 0x4e5470, a message box for the text id with the flags (2: "Messagebox_Autosize", 0x10: "_BIG", 0x20: "_VERY_BIG", 0x100: "_NONE_PERGA" (form 1), else "misc\\Messagebox";
   8: the frame wait flags 0x717c6, 0x80: a timer slider (0x500 ms), 0x40: ends on the first click of the dialog's own click, 4: ends after 500 ms): the dialog's
   objects are put into a radio group (all but the type 0x40), the loop runs the frame wait 0x5309a0 until the stop flag; the answer: the clicked
   button index + 1 (0xf8aad0 == id, the clicked object's result in 0xfa5aa8), else -1; not reentrant (0x6f0dc4). ROUGH */

int ai_Unk4e5470(int id, int flags)
{
    int t0, ret, win, radio, i, n, k, slider, fl, timed, quick;
    int *list;
    char *dlg;

    t0 = ai_Unk6333cc;
    ret = -1;
    win = 0;
    if (ai_Unk6f0dc4 != 0)
        return 0;
    fl = sv_FrameMask & 0xc7;
    if (flags & 2)
        win = d2_OpenForm("misc\\Messagebox_Autosize", 0, 0);
    else if (flags & 0x10)
        win = d2_OpenForm("misc\\Messagebox_BIG", 0, 0);
    else if (flags & 0x20)
        win = d2_OpenForm("misc\\Messagebox_VERY_BIG", 0, 0);
    else if (flags & 0x100) {
        win = d2_OpenForm("misc\\Messagebox_NONE_PERGA", 0, 0);
        d2_SetForm(win, 1);
    } else
        win = d2_OpenForm("misc\\Messagebox", 0, 0);
    if (flags & 8)
        fl = 0x717c6;
    io_PrintText(id);
    d2_CentreViewWindows(fl);
    ai_Unk6f0dc4 = 1;
    radio = d2_CreateRadioGroup(0);
    dlg = d2_LastWindowPtr;
    i = 0;
    if (*(short *)(dlg + 0x1c) > 0) {
        do {
            k = (*(int **)(dlg + 0x18))[i];
            if (*(int *)(d2_Objs + k * 0x2ec + 0x18) != 0x40)
                d2_RadioAdd(radio, k);
            dlg = d2_LastWindowPtr;
            i++;
        } while (i < *(short *)(dlg + 0x1c));
    }
    d2_RadioSelect(radio, 0);
    d2_SetForm(win, 0);
    timed = flags & 0x80;
    if (timed != 0)
        slider = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 0xa) - 0x18, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 0x1f4, 0x83, 0x42, d2_LastWindow);
    else
        slider = -1;
    if (sv_RunFrame(fl, ai_Unk4e5470) != 0) {
        quick = flags & 0x40;
        n = flags & 4;
        do {
            d2_MenuNavigate(radio);
            if (quick != 0)
                d2_RaiseViewWindows(win);
            if (d2_ClickedId == 0) {
                if (d2_HitWindow == id) {
                    ret = d2_RadioSel[radio * 35] + 1;
                    sv_PopRequest = 1;
                } else if (kb_Last == 1) {
                    ret = 0;
                    sv_PopRequest = 1;
                }
            } else if (d2_ClickedId == 1 && d2_HitWindow == id || kb_Last == 1) {
                ret = 0;
                sv_PopRequest = 1;
            }
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (timed != 0 && slider != -1)
                d2_SetObjValue(slider, 0, 0x1f4, ai_Unk6333cc - t0);
            if (n != 0 && ai_Unk6333cc > t0 + 0x1f4)
                sv_PopRequest = 1;
        } while (sv_RunFrame(fl, ai_Unk4e5470) != 0);
    }
    ai_Unk6f0dc4 = 0;
    d2_RadioClear(radio);
    d2_CloseView(win);
    return ret;
}
