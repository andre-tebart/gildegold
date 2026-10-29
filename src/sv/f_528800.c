/* 0x528800, creates a panel button at (x + 3, y) from the image 0xc5 (0x46bf10) with the text, the state 0x54, the flag +0x5c = 1 and the frame 0x60; returns the widget id */

int sv_Unk528800(int x, int y, int text)
{
    int w;

    w = d2_WinCreateSlider(x + 3, y, 0xc5, text, d2_LastWindow);
    d2_SetObjFont(w, 0x54);
    *(int *)(d2_Objs + 0x5c + w * 0x2ec) = 1;
    d2_SetObjWidth(w, 0x60);
    return w;
}
