/* 0x4be180, the dialog "misc\routing_choose_good_enhanced" with the goods of route p (three slots per point,
   loadSlots[2 * k] with a count above 0); returns the good picked, 0 if none
   @deep behaviour: the whole diff is the prologue (17 code hunks, 10 tail nops, 167 equal instructions): the original loads p into eax before the register saves and moves it to esi after the test, stores n's 0 as an immediate; ours loads p straight into esi and stores ebx - same statements, VC6 register allocation/scheduling. Declaration initializers (int n = 0) tried, no change -> deep steers this. Q2 (10 callees/globals) and Q4 (5 raw accesses) stay open */

int rt_Unk4be180(int x, RTPOINT *p)
{
    int n;
    int ret;
    int k;
    int *s;
    int w;
    int ok;
    int i;
    int o;
    int objs[10];
    int goods[24];

    n = 0;
    ret = 0;
    for (; p != 0; p = p->next) {
        s = p->loadSlots;
        for (k = 3; k != 0; k--) {
            if (s[0] != 0 && s[1] > 0)
                goods[n++] = s[0];
            s += 2;
        }
    }
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 4);
    ok = d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 2);
    for (i = 0; i < n; i++) {
        o = d2_WinOpenPic(i % 4 << 6, i / 4 << 6, goods[i] + 0xdf, d2_LastWindow);
        objs[i] = o;
        *(int *)(d2_Objs + o * 0x2ec + 0x4c) = 1;
        *(int *)(d2_Objs + o * 0x2ec + 0x1c0) = 3;
        *(int *)(d2_Objs + o * 0x2ec + 0x1d4) = 0x525;
    }
    goods[i] = 0;
    o = d2_WinOpenPic(i % 4 * 0x40 + 7, i / 4 * 0x40 + 7, 0x527, d2_LastWindow);
    objs[i] = o;
    *(int *)(d2_Objs + o * 0x2ec + 0x4c) = 1;
    *(int *)(d2_Objs + o * 0x2ec + 0x1c0) = 3;
    while (sv_RunFrame(rt_Unk4be180, 0x657c7)) {
        if (dm_L.rup != 0 || ret != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; i < n + 1; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = goods[i];
                    break;
                }
            }
        }
    }
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
