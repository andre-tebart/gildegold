/* 0x472aa0, creates a window inside another one (relative to its scroll position) */

int d2_CreateChildWindow(x, y, pw, h, flags, win)
short x;
short y;
short pw;
short h;
unsigned int flags;
int win;
{
    D2WIN *w = &d2_Wins[win];
    int n;
    int ww, hh;

    if (!w->active)
        return -1;
    n = d2_CreateWindow(w->x - w->scrollX + x, w->y - w->scrollY + y, pw, h, flags);
    d2_AddToDrawOrder(d2_Wins[n].parent, w);
    w->objs[w->nobjs] = d2_Wins[n].parent;
    d2_Objs[d2_Wins[n].parent].win = w;
    d2_Objs[d2_Wins[n].parent].clipy0 = w->y + y;
    d2_Objs[d2_Wins[n].parent].clipy1 = y + h + w->y;
    d2_Objs[d2_Wins[n].parent].clipx0 = w->x + x;
    d2_Objs[d2_Wins[n].parent].clipx1 = w->x + x + pw;
    d2_Objs[d2_Wins[n].parent].buf = d2_Objs[w->parent].buf;
    w->nobjs++;
    d2_GetFrameSize(w, &ww, &hh);
    if (d2_Wins[n].h + y > w->contentHeight)
        w->contentHeight = d2_Wins[n].h + ww * 2 + y;
    return n;
}
