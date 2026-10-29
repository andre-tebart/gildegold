/* 0x46f840, scrolls a window one step (scrollStep pixels) towards its target, follows its scroll bar and moves its
   objects by the change */

void d2_ScrollWindow(int win)
{
    D2WIN *w = &d2_Wins[win];
    D2OBJ *o;
    int i, id, v, f;

    if (w->scrollRemY < 0) {
        w->scrollY -= w->scrollStep;
        w->scrollRemY += w->scrollStep;
        if (w->scrollRemY > 0) {
            w->scrollY += w->scrollRemY;
            w->scrollRemY = 0;
        }
        if (w->scrollY < 0) {
            w->scrollY = 0;
            w->scrollRemY = 0;
        }
    } else if (w->scrollRemY > 0) {
        w->scrollY += w->scrollStep;
        w->scrollRemY -= w->scrollStep;
        if (w->scrollRemY < 0) {
            w->scrollY += w->scrollRemY;
            w->scrollRemY = 0;
        }
        if (w->scrollY > w->contentHeight) {
            w->scrollY = w->contentHeight;
            w->scrollRemY = 0;
        }
    }
    if (w->scrollRemX < 0) {
        w->scrollX -= w->scrollStep;
        w->scrollRemX += w->scrollStep;
        if (w->scrollRemX > 0) {
            w->scrollX += w->scrollRemX;
            w->scrollRemX = 0;
        }
    } else if (w->scrollRemX > 0) {
        w->scrollX += w->scrollStep;
        w->scrollRemX -= w->scrollStep;
    }
    if (w->scrollBar != -1) {
        v = d2_GetObjValue(w->scrollBar);
        if (d2_Objs[w->scrollBar].u.s.flags & 1)
            w->scrollY = (int)((float)(w->contentHeight - w->h) * (100 - v) * 0.01f);
        if (w->contentHeight < w->h)
            d2_EnableObject(w->scrollBar, 0);
        else if (!d2_Objs[w->parent].disabled)
            d2_EnableObject(w->scrollBar, 1);
        w->scrollY = w->scrollY < 0 ? 0 : w->scrollY;
    }
    if (w->scrollY != w->appliedScrollY || w->scrollX != w->appliedScrollX) {
        f = w->flags;
        if ((char)f & 8)
            i = 1;
        else
            i = 0;
        if (f & 0x20)
            i += 2;
        for (; i < w->nobjs; i++) {
            id = w->objs[i];
            o = &d2_Objs[id];
            d2_MoveObj(o->x - w->scrollX + w->appliedScrollX, o->y - w->scrollY + w->appliedScrollY, id);
            if (o->type == 0x11)
                o->input = 2;
        }
    }
    w->appliedScrollY = w->scrollY;
    w->appliedScrollX = w->scrollX;
}
