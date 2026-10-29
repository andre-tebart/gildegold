/* 0x475b80, k-th object of window idx (of a view if view != -1); -1 if there is none */

int d2_GetWinObj(int view, int idx, int k)
{
    D2WIN *w;

    if (view != -1)
        w = &d2_Wins[d2_Views[view].wins[idx]];
    else
        w = &d2_Wins[idx];
    if (k > w->nobjs)
        return -1;
    return w->objs[k];
}
