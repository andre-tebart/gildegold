/* 0x475ff0, window i of a view, -1 if out of range */

#define WIN(i) ((D2PAINTWIN *)&d2_Wins[i])

int d2_GetViewWin(int view, int i)
{
    if (i <= d2_Views[view].nwins && i >= 0)
        return d2_Views[view].wins[i];
    return -1;
}
