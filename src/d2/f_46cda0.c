/* 0x46cda0 */

/* 0x46cda0, moves an object (and, for a window, the objects on it) to x, y and recomputes its clip rectangle */
void d2_MoveObj(short x, short y, int obj);

void d2_MoveObj(short x, short y, int obj)
{
    D2OBJ *o;
    D2WIN *win, *w;
    int type, n, i, id;
    int oldx, oldy;

    type = d2_Objs[obj].type;
    o = &d2_Objs[obj];
    o->x = x;
    o->y = y;
    o->shadowX = 8;
    o->shadowY = 8;
    if (type != 0x40) {
        o->clipy0 = 0;
        o->clipy1 = d2_ScreenH;
        o->clipx0 = 0;
        o->clipx1 = d2_ScreenW;
    } else {
        o->clipy0 = y;
        o->clipy1 = o->h + y;
        o->clipx0 = x;
        o->clipx1 = o->w + x;
    }
    win = o->win;
    if (win) {
        o->clipy0 = max(win->y, o->clipy0);
        o->clipy1 = min(win->h + win->y, o->clipy1);
        o->clipy1 = min(o->clipy1, d2_ScreenH);
        o->clipx0 = max(win->x, o->clipx0);
        o->clipx1 = min(win->w + win->x, o->clipx1);
        o->clipx1 = min(o->clipx1, d2_ScreenW);
    }
    switch (type) {
    case 0x41:
        d2_Inputs[o->input].x = x;
        d2_Inputs[o->input].y = o->y;
        break;
    case 0x40:
        n = o->input;
        w = &d2_Wins[n];
        oldx = w->x;
        oldy = w->y;
        w->y = y;
        w->x = x;
        if (o->clipx1 > d2_ScreenW)
            o->clipx1 = d2_ScreenW;
        if (o->clipy1 > d2_ScreenH)
            o->clipy1 = d2_ScreenH;
        for (i = 0; i < w->nobjs; i++) {
            id = w->objs[i];
            d2_MoveObj(d2_Objs[id].x - oldx + w->x, d2_Objs[id].y - oldy + w->y, id);
        }
        break;
    case 4:
        d2_AnimView[o->input].x = x;
        d2_AnimView[o->input].y = o->y;
        break;
    }
}
