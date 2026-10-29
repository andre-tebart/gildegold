/* 0x473350, the same without the horizontal scroll, and with a style */

int d2_WinOpenPicStyled(int x, int y, int pic, int win, int a)
{
    D2WIN *w = &d2_Wins[win];
    int obj;


    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    w->objs[w->nobjs] = d2_CreatePicObj(w->x + x, w->y - w->scrolly + y, pic);
    obj = w->objs[w->nobjs];
    if (obj != -1) {
        d2_AddToDrawOrder(obj, w);
        d2_Objs[obj].win = w;
        d2_Objs[obj].clipy0 = 0;
        d2_Objs[obj].clipy1 = d2_ScreenH;
        d2_Objs[obj].clipx0 = 0;
        d2_Objs[obj].clipx1 = d2_ScreenW;
        d2_Objs[obj].buf = d2_Objs[w->parent].buf;
        d2_Objs[obj].disabled = d2_Objs[w->parent].disabled;
        d2_ScaleObject(obj, a);
        w->nobjs++;
        d2_GetFrameSize(w, &x, &win);
        if ((short)y + d2_Objs[obj].h > w->contentHeight)
            w->contentHeight = (short)y + x * 2 + d2_Objs[obj].h;
    }
    return obj;
}
