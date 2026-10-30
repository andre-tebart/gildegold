/* 0x4be790, the controls of one good slot (k 0..2) of a route point: a slider (0x50ab50(a, good) steps) and the
   good's picture with its name; out = {-1, picture, slider}; returns the picture
   @deep behaviour: frame matches (0x24) and every statement corresponds (offs inits, the slider 0x50ab50, d2_WinOpenSlider(x-8, y+0x1d, 0x2f, 0, s, 0x6a, 0x2d, win), d2_WinOpenInputIcon(x, y+0x22, good+0xdf, 0x8a, win), the out[] stores and the d2_Objs fields); the 102 code hunks are register allocation and scheduling (ebp/ebx/eax swaps, the two open calls interleaved, offs stores reordered) - misalignment, no statement differs. Q2 (ai_Unk143c860, gm_ListStock, d2_SetObjFont) and Q4 (4) stay open */

int rt_Unk4be790(unsigned short *bt, int a, int good, int k, int x, int y, int win, int *out)
{
    int offs[9];
    int s;
    int o;

    offs[1] = 0;
    offs[2] = 0;
    offs[5] = 0;
    offs[0] = 0x51;
    offs[3] = 0x30;
    offs[4] = 0x71;
    offs[6] = 0x11;
    offs[7] = 0x52;
    offs[8] = 0x93;
    s = gm_ListStock(a, good);
    x = offs[(*bt - 0x134) * 3 + k] + 0x6e + x;
    s = d2_WinOpenSlider(x - 8, y + 0x1d, 0x2f, 0, s, 0x6a, 0x2d, win);
    o = d2_WinOpenInputIcon(x, y + 0x22, good + 0xdf, 0x8a, win);
    d2_SetObjFont(o, 0x54);
    out[0] = -1;
    out[1] = o;
    out[2] = s;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1c0) = 0x22;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1cc) = 4;
    *(int *)(d2_ObjsC + o * 0x2ec + 0x1d0) = 0xf;
    sprintf(d2_ObjsC + o * 0x2ec + 0xbc, "%s", *(char **)(ai_Unk143c860 + good * 8));
    return o;
}
