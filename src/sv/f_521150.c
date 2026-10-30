/* 0x521150, a modal message box (the form "misc\\Messagebox" in the sizes by the flags 2 Autosize / 0x10 BIG / 0x20 VERY_BIG, 0x100 the pergament "NONE_PERGA", 8 all messages): shows the text, loops
   sv_RunFrame until a button, waits at most 500 ms with flag 4, a countdown bar with flag 0x80 and returns the button + 1 (0 for none / cancel). Not reentrant (sv_MessageBoxOpen, 0x6f2350)
   @deep behaviour: 133 code hunks, every hunk read against the original; the statement set is complete (form call per branch, timed/quick/n, do-while frame loop, unsigned t0 timeout) and the rest is the allocation cascade: the original holds the widget-table base in ebx, so the flags param slot stays live and form/ret/bar take the slots frame+0x00/+0x08/+0x10 with bar in esi; ours puts the table in edx, form in the flags slot, bar in a slot and keeps a zero register in esi (the cmp eax,esi / test eax,eax / cmp DWORD PTR ds:X,esi hunks follow from that, not from a meaning error). Readability as far as base evidence goes: callees d2_OpenForm / d2_SetForm / d2_RadioAdd / d2_RadioSelect / d2_RaiseViewWindows / d2_WinOpenSlider / d2_RadioSel / d2_ClickedId / d2_SetObjValue and globals kb_Last / d3_Time / d2_LastWindowPtr / d2_Objs / sv_MessageBoxOpen named, body complete and described; open: Q2 d2_HitWindow (0xf8aad0, gfx.h calls it d2_Unk8aad0, still a placeholder; role = the window under the pointer copied from d2_PointerWin in d2_MouseUpdate, a d2.h naming decision); Q4 7 raw accesses need D2WIN / D2OBJ typed views (sv.c cannot include d2.h: its vfs_Root / vfs_Tfopen clash with vfs.h); Q7 d2_SetObjValue is K&R in d2.h and cm.h because the callers pass 2..5 arguments, no canonical prototype. */

int sv_MessageBox(int text, unsigned flags)
{
    int ret, form, bar, idx, i, k, scene, timed, quick, n;
    unsigned mask;
    unsigned t0;

    t0 = d3_Time;
    ret = 0;
    i = 0;
    bar = -1;
    if (sv_MessageBoxOpen != 0)
        return 0;
    mask = sv_FrameMaskU & 0xffffffc7;
    if (flags & 2)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_Autosize");
    else if (flags & 0x10)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if (flags & 0x20)
        form = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else if (flags & 0x100) {
        form = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
        d2_SetForm(form, 1);
    } else
        form = d2_OpenForm(0, 0, "misc\\Messagebox");
    if (flags & 8)
        mask = 0x717c6;
    io_PrintText(text);
    scene = d2_LastWindow;
    d2_CentreViewWindows(form);
    sv_MessageBoxOpen = 1;
    idx = d2_CreateRadioGroup(0);
    k = d2_LastWindowPtr;
    if (*(short *)(k + 0x1c) > 0) {
        do {
            int w = *(int *)(*(int *)(k + 0x18) + i * 4);
            if (*(int *)(d2_Objs + 0x18 + w * 0x2ec) != 0x40)
                d2_RadioAdd(idx, w);
            k = d2_LastWindowPtr;
            i++;
        } while (i < *(short *)(k + 0x1c));
    }
    d2_RadioSelect(idx, 0);
    d2_SetForm(form, 0);
    timed = flags & 0x80;
    if (timed != 0)
        bar = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtr + 10) - 0x18, *(short *)(d2_LastWindowPtr + 8) - 0x40, 0, 500, 0x83, 0x42, d2_LastWindow);
    if (sv_RunFrame(sv_MessageBox, mask) != 0) {
        quick = flags & 0x40;
        n = flags & 4;
        do {
            d2_MenuNavigate(idx);
            if (quick != 0)
                d2_RaiseViewWindows(form);
            if (d2_ClickedId == 0) {
                if (d2_HitWindow == scene) {
                    ret = d2_RadioSel[idx * 0x23] + 1;
                    sv_PopRequest = 1;
                } else if (kb_Last == 1) {
                    ret = 0;
                    sv_PopRequest = 1;
                }
            } else if ((d2_ClickedId == 1 && d2_HitWindow == scene) || kb_Last == 1) {
                ret = 0;
                sv_PopRequest = 1;
            }
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (timed != 0 && bar != -1)
                d2_SetObjValue(bar, 0, 500, d3_Time - t0);
            if (n != 0 && d3_Time > t0 + 500)
                sv_PopRequest = 1;
        } while (sv_RunFrame(sv_MessageBox, mask) != 0);
    }
    sv_MessageBoxOpen = 0;
    d2_RadioClear(idx);
    d2_CloseView(form);
    return ret;
}
