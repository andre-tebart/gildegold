/* 0x476f00, draws the pointer at the mouse position plus x, y */

int d2_DrawMousePointer(int x, int y)
{
    int a = 6, b = 6;
    int px = d2_MouseX + x;
    int py = d2_MouseY + y;
    if (d2_PointerPic == 0)
        d2_PointerPic = d2_GetPic(d2_FindPic("_MOUSE_CURSOR"));
    if (dm_L.ldown != 0) {
        px--;
        py++;
        a = 7;
        b = 5;
    }
    d2_DrawPointer(d2_ScreenSurf, px, py, a, b);
    return 1;
}
