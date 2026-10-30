/* 0x4bdcd0, the dialog "misc\routing_choose_good_enhanced": the goods of building b to pick from (its stock of a
   market (type 0x1c, more than 1) / warehouse (type 9), else the last list with the stock of b), plus "none"
   (picture 0x527); returns the good picked, def if none
   @deep behaviour: the diff read end to end is one instruction stream shifted (ours 0xf bytes shorter): register allocation (ebx/ebp/esi), argument spills and stack slots (the original keeps b in [esp+0xc] and reads it at the end, ours keeps it in esi), tail nop fill - no statement differs; the 497 code hunks are the misalignment this causes. Needs the deep slot/register steering. Q2 (17 callees/globals) and Q4 (23 raw accesses, D2OBJ/d2_Objs view) stay open */

int rt_Unk4bdcd0(unsigned char *b, int x, int def)
{
    char *t;
    int w;
    char *s;
    unsigned short *g;
    char *e;
    unsigned int n;
    int i;
    int o;
    int ret;
    int objs[10];

    t = ai_BTypes + *b * 0x24d;
    w = d2_OpenForm(0, 0, "misc\\routing_choose_good_enhanced");
    d2_CentreViewWindows(w);
    d2_SetForm(w, 4);
    d2_GetWinObj(w, 4, io_PrintText(0x7d));
    d2_SetForm(w, 2);
    if (*t == 0x1c) {
        s = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x1dc, 6);
        n = 0;
        if (s != 0) {
            for (g = gm_Query(*(int *)(s + 0x14), 1, 5); g != 0; g = gm_QueryNext()) {
                if (*(int *)(g + 7) > 1) {
                    o = d2_WinOpenPic((int)n % 4 << 6, (int)n / 4 << 6, *g + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                    ai_Unk889400[n] = *g;
                    n++;
                }
            }
        }
    } else if (*t == 9) {
        s = gm_Query(*(int *)(b + 0x5d), 2, 0, 0x116, 6);
        n = 0;
        if (s != 0) {
            for (g = gm_Query(*(int *)(s + 0x14), 1, 5); g != 0; g = gm_QueryNext()) {
                if (*(int *)(g + 7) > 0) {
                    o = d2_WinOpenPic((int)n % 4 << 6, (int)n / 4 << 6, *g + 0xdf, d2_LastWindow);
                    objs[n] = o;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
                    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                    ai_Unk889400[n] = *g;
                    n++;
                }
            }
        }
    } else {
        gm_BuildKindObjects(b);
        s = gm_Query(*(int *)(b + 0x5d), 2, 0, 0xff, 6);
        n = 0;
        for (i = 0; i < ai_Unk76fdc4; i++) {
            e = gm_Query(*(int *)(s + 0x14), 1, 0, ai_Unk889400[n]);
            if (e != 0 && *(int *)(e + 0xe) > 1) {
                o = d2_WinOpenPic((int)n % 4 << 6, (int)n / 4 << 6, ai_Unk889400[n] + 0xdf, d2_LastWindow);
                objs[n] = o;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 0x23;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1d4) = 0x525;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1cc) = 4;
                *(int *)(d2_ObjsC + o * 0x2ec + 0x1d0) = 0xf;
                sprintf(d2_ObjsC + o * 0x2ec + 0xbc, "%s", *(char **)(ai_Unk143c860 + ai_Unk889400[n] * 8));
                n++;
            }
        }
    }
    ai_Unk76fdc4 = (unsigned short)n;
    ai_Unk889400[n] = 0;
    o = d2_WinOpenPic((int)n % 4 * 0x40 + 7, (int)n / 4 * 0x40 + 7, 0x527, d2_LastWindow);
    objs[n] = o;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x4c) = 1;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 3;
    ret = def;
    while (sv_RunFrame(rt_Unk4bdcd0, 0x657c7)) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; i < ai_Unk76fdc4 + 1; i++) {
                if (d2_PointerObj == objs[i]) {
                    ret = ai_Unk889400[i];
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
    }
    d2_CloseView(w);
    di_ClearButtons();
    return ret;
}
