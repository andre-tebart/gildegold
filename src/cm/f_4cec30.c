/* 0x4cec30, opens a window (text box 110+ wide) around the model box of the character c; Rough draft */

void ai_Unk4cec30(int a, char *c)
{
    int v[8];
    int x0, y0, w, h, i, hnd;

    if (d3if_ScreenBox(*(int *)(*(char **)(c + 0x184) + 0x34), v) == 0)
        return;
    x0 = v[4] - 0x10;
    w = v[6] - v[4] + 0x20;
    y0 = v[5] - 0x10;
    if (w <= 0x6e)
        w = 0x6e;
    h = v[7] - v[5] + 0x20;
    for (i = 0; i < 0x20; i++) {
        if (ai_Unk935d20[i] == -1)
            break;
    }
    if (i >= 0x20)
        return;
    ai_Unk4cdf00((char *)a, c);
    ai_Unk935d20[i] = d2_CreateWindow(x0, y0, w, h, 0x10);
    d2_WinOpenPic(0, 0, 0x7b0, d2_LastWindow);
    d2_WinOpenPic(w - 0x18, 0, 0x7b1, d2_LastWindow);
    hnd = d2_LastWindow;
    *(short *)(ai_Unkf8f39c + hnd * 0x3c4) = 0x54;
    sv_Unk52eaa0K(4, 4, gm_Unk504e00(c, 3), 0x4df);
    hnd = (int)(unk_Unk4fc8e0(c) * 100.0);
    d2_WinOpenSlider(0, 0xe, 0x40, 0, 0x64, 0x75, 0x442, d2_LastWindow);
    d2_SetObjValue(d2_LastWindow, 0, 0x64, hnd);
}
