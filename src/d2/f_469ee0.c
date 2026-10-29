/* 0x469ee0, the same on a window */

int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj;

    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_OpenInputIcon(w->x + x, w->y - w->scrollY + y, icon, flags);
    d2_AddToDrawOrder(obj, w);
    d2_Objs[obj].win = w;
    d2_Objs[obj].buf = d2_Objs[w->parent].buf;
    d2_Inputs[d2_Objs[obj].input].font = w->font;
    w->objs[w->nobjs] = obj;
    w->nobjs++;
    if (y > w->contentHeight)
        w->contentHeight = y + 4;
    return obj;
}
