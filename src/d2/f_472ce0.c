/* 0x472ce0, removes the object of a window */

int d2_RemoveWindow(win)
int win;
{
    if (d2_Wins[win].active) {
        d2_RemoveObj(d2_Wins[win].parent);
        return 1;
    }
    return 0;
}
