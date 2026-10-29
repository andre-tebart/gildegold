/* 0x468cf0, puts an object on a window; 0 if it is full or the window does not exist */

int d2_AddToWin(int obj, int win)
{
    D2WIN *w;
    int t;

    if (win >= 0 && win <= 0x60) {
        w = &d2_Wins[win];
        if (w->active) {
            if (w->nobjs >= 0x180) {
                err_Message("Too many objects on window!");
                return 0;
            }
            d2_AddToDrawOrder(obj, w);
            d2_Objs[obj].win = w;
            d2_Objs[obj].buf = d2_Objs[w->parent].buf;
            d2_Objs[obj].clipy0 = w->y;
            d2_Objs[obj].clipy1 = w->h + w->y;
            d2_Objs[obj].clipx0 = w->x;
            d2_Objs[obj].clipx1 = w->w + w->x;
            d2_Objs[obj].disabled = d2_Objs[w->parent].disabled;
            t = (d2_Objs[obj].h + d2_Objs[obj].y) - w->y;
            if (t > w->contentHeight)
                w->contentHeight = t + 4;
            w->objs[w->nobjs] = obj;
            w->nobjs++;
            return 1;
        }
    }
    return 0;
}
