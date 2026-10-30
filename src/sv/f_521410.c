/* 0x521410, the green message box ("misc\\MessageBoxGreen" 0x623ed0) with an optional title text a and the text b; like 0x521150, a button returns its number + 1. Not reentrant (sv_MessageBoxGreenOpen, 0x6f2351) @deep behaviour: 73 code hunks, all read: the statement set is complete and matches 0x521150's twin (form, a/b texts, timed/quick/n, do-while frame loop); the residue is allocation - the original keeps the constant 1 in ebx (cmp eax,ebx / mov ds:X,ebx / cmp BYTE PTR ds:X,bl) and hoists bar = -1 into its own frame+0x10 slot with form in ebp, ours uses immediates and puts form in edi (the first attempts at the hoist made the count worse: 73 -> 267, reverted). Readability as far as base evidence goes: sv_FitForm (0x52ea30), d2_OpenForm / d2_SetForm / d2_RadioAdd / d2_RadioSelect / d2_RaiseViewWindows / d2_WinOpenSlider / d2_RadioSel / d2_ClickedId / d2_SetObjValue, kb_Last / ai_Unk6333cc / d2_LastWindowPtr / d2_Objs and sv_MessageBoxGreenOpen named, marker dropped; open: Q4 6 raw accesses need the D2WIN / D2OBJ typed views (d2.h cannot be included in sv.c: vfs_Root / vfs_Tfopen clash with vfs.h); Q7 d2_SetObjValue K&R (2..5 argument callers, no canonical prototype). */

int sv_MessageBoxGreen(int a, int b, unsigned flags)
{
    int ret, form, bar, idx, i, k;
    unsigned mask;
    unsigned t0;

    t0 = d3_Time;
    i = 0;
    ret = 0;
    if (sv_MessageBoxGreenOpen != 0)
        return 0;
    mask = sv_FrameMaskU & 0xffffffc7;
    form = d2_OpenForm(0, 0, "misc\\MessageBoxGreen");
    if ((flags & 8) != 0)
        mask = 0x717c6;
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    if (a != 0)
        io_PrintText(a);
    d2_SetForm(form, 2);
    io_PrintText(b);
    d2_CentreViewWindows(form);
    sv_MessageBoxGreenOpen = 1;
    idx = d2_CreateRadioGroup(0);
    k = d2_LastWindowPtr;
    while (i < *(short *)(k + 0x1c)) {
        int w = *(int *)(*(int *)(k + 0x18) + i * 4);
        if (*(int *)(d2_Objs + 0x18 + w * 0x2ec) != 0x40) {
            d2_RadioAdd(idx, w);
            k = d2_LastWindowPtr;
        }
        i++;
    }
    d2_RadioSelect(idx, 0);
    d2_SetForm(form, 0);
    if ((flags & 0x80) == 0)
        bar = -1;
    else
        bar = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 10) - 0x18, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 500, 0x83, 0x42, d2_LastWindow);
    while (sv_RunFrame(sv_MessageBoxGreen, mask) != 0) {
        d2_MenuNavigate(idx);
        if ((flags & 0x40) != 0)
            d2_RaiseViewWindows(form);
        if (d2_ClickedId == 0) {
            sv_PopRequest = 1;
            ret = d2_RadioSel[idx * 0x23] + 1;
        } else if (d2_ClickedId == 1 || kb_Last == 1) {
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
    sv_MessageBoxGreenOpen = 0;
    d2_RadioClear(idx);
    d2_CloseView(form);
    return ret;
}
