/* 0x46c390, the same on a window */

int d2_WinOpenRaw(int x, int y, int w, int h, void *src, int win)
{
    int obj;

    obj = d2_OpenRaw(d2_Wins[win].x + x, d2_Wins[win].y + y, w, h, src);
    d2_AddToWin(obj, win);
    return obj;
}
