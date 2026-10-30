/* 0x4be3b0, the dialog "misc\routing_choose_good_enhanced" for building b with seven tabs (stock kinds 0x92..0x97,
   0x2db; text "$Z%2N3$A%1G"): the goods of the tab picked; returns the good picked, def if none
   @deep behaviour: frame 0x1d0 vs the original 0x2e4 (0x114 = 69 ints): the local arrays are too small/misordered (ours kinds/objs/goods/tabs land at +0x34/0x50/0x120/0x128, the original's at +0x44/0x54/0x104/0x1d0) and the s==0 arm differs in which value is pushed ([eax+0x14] vs [emp+0x5d] alignment) - the diff was read end to end; a real reread of the locals and that branch is needed. Q2/Q4 stay open */

unsigned int rt_Unk4be3b0(char *b, int x, unsigned int def)
{
    int first;
    int last;
    int kinds[7];
    int objs[0x30];
    unsigned int goods[0x30];
    int tabs[7];
    unsigned int ret;
    int w;
    int ok;
    int g;
    int i;
    int y;
    char *s;
    unsigned short *e;
    int n;
    int o;

    ret = def;
    first = 1;
    last = -1;
    kinds[0] = 0x95;
    kinds[1] = 0x92;
    kinds[2] = 0x93;
    kinds[3] = 0x94;
    kinds[4] = 0x97;
    kinds[5] = 0x96;
    kinds[6] = 0x2db;
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 1);
    io_PrintText((int)"$Z%2N3$A%1G", *(unsigned short *)(b + 0x27), b);
    d2_SetForm(w, 4);
    ok = d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 3);
    g = d2_CreateRadioGroup(0);
    for (i = 0, y = 4; y < 0xd6; i++, y += 0x1e) {
        o = sv_Unk528800(0, y, ai_Unk143a588[i]);
        tabs[i] = o;
        *(void **)(d2_ObjsC + o * 0x2ec + 0x2e8) = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, kinds[i]);
        d2_RadioAdd(g, o);
    }
    d2_RadioSelect(g, 0);
    d2_ClearInput();
    for (;;) {
        if (dm_L.rup != 0 || ret != 0 || d2_ClickedId != -1 && d2_PointerObj == ok)
            sv_PopRequest = 1;
        if (d2_RadioSel[g * 0x23] != last || first) {
            d2_SetForm(w, 2);
            d2_RemoveWindowObjects(d2_GetViewWin(w, 2), 0);
            for (i = 0; i < 0x30; i++)
                objs[i] = -1;
            n = 0;
            s = *(char **)(d2_ObjsC + tabs[d2_RadioSel[g * 0x23]] * 0x2ec + 0x2e8);
            if (s == 0)
                s = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xff);
            else
                s = gm_Query(*(int *)(s + 0x14), 1, 0, 0xff);
            if (s != 0) {
                for (e = gm_Query(*(int *)(s + 0x14), 1, 5); e != 0; e = gm_QueryNext()) {
                    o = d2_WinOpenPic(n % 4 << 6, n / 4 << 6, *e + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                    goods[n] = *e;
                    n++;
                }
            }
            goods[n] = 0;
            o = d2_WinOpenPic(n % 4 * 0x40 + 7, n / 4 * 0x40 + 7, 0x527, d2_LastWindow);
            objs[n] = o;
            first = 0;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
            *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
            last = d2_RadioSel[g * 0x23];
        }
        if (d2_ClickedId != -1) {
            for (i = 0; i < 0x30; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = goods[i];
                    break;
                }
            }
        }
        if (!sv_RunFrame(rt_Unk4be3b0, 0x657c7))
            break;
    }
    d2_RadioClear(g);
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
