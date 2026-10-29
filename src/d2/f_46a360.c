/* 0x46a360, the same on a window */

int d2_WinOpenInput(int x, int y, int w, int h, unsigned char flags, int win)
{
    D2WIN *wn = &d2_Wins[win];
    int obj;

    if (!wn->active)
        return -1;
    if (wn->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_OpenInput(wn->x + x, wn->y - wn->scrollY + y, w, h, flags);
    d2_AddToDrawOrder(obj, wn);
    d2_Objs[obj].win = wn;
    d2_Objs[obj].buf = d2_Objs[wn->parent].buf;
    d2_Objs[obj].clipy0 = wn->y;
    d2_Objs[obj].clipy1 = wn->h + wn->y;
    d2_Objs[obj].clipx0 = wn->x;
    d2_Objs[obj].clipx1 = wn->x + wn->w;
    d2_Inputs[d2_Objs[obj].input].font = wn->font;
    d2_Inputs[d2_Objs[obj].input].maxw = wn->x - d2_Objs[obj].x - wn->mr + wn->w;
    wn->objs[wn->nobjs] = obj;
    wn->nobjs++;
    return obj;
}
