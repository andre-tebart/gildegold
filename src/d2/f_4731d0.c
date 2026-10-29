/* 0x4731d0, creates a picture object on a window (position scrolled in both directions) */

/* the callers above pass shorts (see d2.h), the calls below take them too */
int d2_CreatePicObj(short x, short y, int pic);  /* @original: K&R definition (movsx reads) with a short prototype for its callers (16-bit arithmetic in d2_WinOpenPic); C89 forbids the pair, the bytes need it */

int d2_WinOpenPic(short x, short y, int pic, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj, bw, bh;

    bw = 0;
    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    w->objs[w->nobjs] = d2_CreatePicObj(w->x - w->scrollx + x, w->y - w->scrolly + y, pic);
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
        w->nobjs++;
        d2_GetFrameSize(w, &bw, &bh);
        if ((short)y + d2_Objs[obj].h > w->contentHeight)
            w->contentHeight = (short)y + bw * 2 + d2_Objs[obj].h;
    }
    return obj;
}
