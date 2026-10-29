/* 0x46e460, the object under the pointer: sets d2_PointerObj / 714 / 72c / 730, returns its unk08 */

int d2_HitTest(short x, short y)
{
    int i, busy, ret;
    int x0, x1, y0, y1;
    D2OBJ *o, *p;
    D2WIN *win;

    d2_FindWindowAt(x, y);
    d2_PointerWinVisible = -1;
    d2_HitObjId = -1;
    for (i = 0x2ff; i >= 0; i--) {
        busy = 0;
        o = d2_DrawOrder[i];
        if (o) {
            if (o->buf)
                busy = o->buf->loaded;
            if (x >= o->x && x <= o->x + o->w && y >= o->y && y <= o->y + o->h && !busy
                && y >= o->clipy0 && y <= o->clipy1 && !o->clickDisabled && !o->disabled && d2_Objs[o->id].type == 0x40
                && d2_Wins[o->input].visible) {
                d2_PointerWinVisible = o->input;
                break;
            }
        }
    }
    for (i = 0x2ff; i >= 0; i--) {
        o = d2_DrawOrder[i];
        busy = 0;
        if (o) {
            if (o->buf)
                busy = o->buf->loaded;
            if (x >= o->x && x <= o->x + o->w && y >= o->y && y <= o->y + o->h && !busy) {
                win = o->win;
                if (win) {
                    p = &d2_Objs[win->parent];
                    x0 = win->x > p->clipx0 ? win->x : p->clipx0;
                    x1 = win->x + win->w < p->clipx1 ? win->x + win->w : p->clipx1;
                    y0 = win->y > p->clipy0 ? win->y : p->clipy0;
                    y1 = win->y + win->h < p->clipy1 ? win->y + win->h : p->clipy1;
                } else {
                    x0 = o->clipx0;
                    x1 = o->clipx1;
                    y0 = o->clipy0;
                    y1 = o->clipy1;
                }
                if (y >= y0 && y <= y1 && x >= x0 && x <= x1 && !o->disabled
                    && (o->interactive || o->checkable || o->type == 0x41 || o->hitTestable)) {
                    if (!o->clickDisabled) {
                        d2_HitObjId = d2_PointerObj = o->id;
                        if (o->win)
                            d2_PointerWin = o->win->id;
                        if (o->type == 9)
                        {
                            if ((char)o->style & 0x10)
                                return 1;
                            return 0;
                        }
                        return o->pic;
                    }
                    if (d2_HitObjId == -1)
                        d2_HitObjId = o->id;
                    d2_PointerObj = -1;
                }
            }
        }
    }
    ret = d2_HitTestViews(x, y);
    if (ret == -1)
        ret = d2_PointerObj = -1;
    return ret;
}
