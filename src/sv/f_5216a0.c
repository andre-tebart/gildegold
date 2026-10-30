/* 0x5216a0, the yes / no message box (the form by the size flags like 0x521150: 2 Autosize, 0x10 BIG, 0x20 VERY_BIG):
   the text, a radio button per non-static window of the form, the optional slider 0x80 and the expiry check 4; returns 1
   for the first button (yes), 0 for the second. Not reentrant: it holds sv_MessageBoxYesNoOpen (0x6f2352) while it runs, and
   returns 0 at once when the box is already open
   @deep B3: code hunks remain (grade: 114 code, 24 slots, 2 fill; 90 differing asm lines, 394/608 bytes, our size 608 vs the original 607). The whole original 0x5216a0..0x5218fe was read (disasm) and the statement sequence matches: the non-reentrancy guard, mask = sv_FrameMask & 0xffffffc7 with the 8 -> 0x717c6 override, the form per size flag, io_PrintText / d2_CentreViewWindows / d2_CreateRadioGroup, the radio-button walk over the non-static windows, d2_RadioSelect / d2_SetForm, the optional slider 0x80, and the do-while sv_RunFrame body (d2_MenuNavigate, 0x40 raise, clicked 0 -> ret 1 / clicked 1 -> ret 0, the mouse-event pop, the slider update and the 500 ms expiry). Fixes this session: the form name is one d2_OpenForm call per arm (if ((flags & 2) != 0) ... else if 0x10 ... else if 0x20 ...; the original pushes the string per arm and shares the push 0 / push 0 / call tail, 246 -> 114 code hunks), the clock is d3_Time (the canonical 0x6333cc name; the volatile re-read matches the original's re-reads), sv_MessageBoxYesNoOpen (0x6f2352) named after its twin flags sv_MessageBoxOpen / sv_MessageBoxGreenOpen, Q6 comment, ROUGH dropped. Left, all allocation: (1) the shared constant 0x40 - ours hoists mov edi,0x40 and reuses it for the window style compare AND the slider's height - 0x40, and reuses edi for the constant 1 later; the original uses the immediates in all three places and never touches edi (0x52174d / 0x521772 / 0x5217dd / 0x521840); (2) the slot pair: ours bar at frame+0x10 and t0 at frame+0xc, the original bar at frame+0xc and t0 at frame+0x10 (0x5216a9 / 0x5216b4), and the push edi shifts every other slot by 4; declaration order tried (bar before t0, t0 first, w at the top) with no change; (3) bar lives in esi in ours, in a slot in the original (0x521814 / 0x521840 region). Q1-Q3, Q5, Q6 pass; Q4 (6 raw *(T *) in the d2 window / obj reads) and Q7 (d2_SetObjValue K&R) stay the documented type debt of the whole sv.c message-box family (sv.c cannot include d2.h: vfs_Root / vfs_Tfopen clash with vfs.h), same as the note of 0x521150 / 0x521410. */

int sv_MessageBoxYesNo(int text, unsigned flags)
{
    int t0, ret, form, bar, idx, i, k, w;
    unsigned mask;

    t0 = d3_Time;
    i = 0;
    bar = -1;
    ret = 0;
    if (sv_MessageBoxYesNoOpen != 0)
        return 0;
    mask = sv_FrameMask & 0xffffffc7;
    if ((flags & 2) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_Autosize");
    else if ((flags & 0x10) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if ((flags & 0x20) != 0)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else
        form = d2_OpenForm(0, 0, "misc\\Messagebox");
    if ((flags & 8) != 0)
        mask = 0x717c6;
    io_PrintText(text);
    d2_CentreViewWindows(form);
    sv_MessageBoxYesNoOpen = 1;
    idx = d2_CreateRadioGroup(0);
    k = d2_LastWindowPtr;
    while (i < *(short *)(k + 0x1c)) {
        w = *(int *)(*(int *)(k + 0x18) + i * 4);
        if (*(int *)(d2_Objs + 0x18 + w * 0x2ec) != 0x40) {
            d2_RadioAdd(idx, w);
            k = d2_LastWindowPtr;
        }
        i++;
    }
    d2_RadioSelect(idx, 0);
    d2_SetForm(form, 0);
    if ((flags & 0x80) != 0)
        bar = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 10) - 0x20, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 500, 0x83, 0x42, d2_LastWindow);
    while (sv_RunFrame(sv_MessageBoxYesNo, mask) != 0) {
        d2_MenuNavigate(idx);
        if ((flags & 0x40) != 0)
            d2_RaiseViewWindows(form);
        if (d2_ClickedId == 0) {
            sv_PopRequest = 1;
            ret = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
            ret = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if ((flags & 0x80) != 0 && bar != -1)
            d2_SetObjValue(bar, 0, 500, d3_Time - t0);
        if ((flags & 4) != 0 && t0 + 500 < d3_Time)
            sv_PopRequest = 1;
    }
    sv_MessageBoxYesNoOpen = 0;
    d2_RadioClear(idx);
    d2_CloseView(form);
    return ret;
}
