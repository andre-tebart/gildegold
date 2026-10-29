/* 0x46a960, the same on a window */

int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win)
{
    D2WIN *w;
    int obj;

    if (win >= 0 && win <= 0x60 && (w = &d2_Wins[win])->active) {
        if (w->nobjs >= 0x180) {
            err_Message("Too many objects on window!");
        } else {
            obj = d2_OpenSlider(w->x + x, w->y - w->scrollY + y, len, p4, p5, pic, flags);
            d2_AddToWin(obj, win);
            return obj;
        }
    }
    return -1;
}
