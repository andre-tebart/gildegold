/* 0x472e00, closes a window and everything on it; 0 if there is no such window */

int d2_CloseWindow(int win)
{
    D2WIN *w;

    if (win > 0x60)
        return 0;
    w = &d2_Wins[win];
    if (w->active == 0)
        return 0;
    while (w->nobjs != 0)
        d2_RemoveObj(w->objs[0]);
    if (w->objs) {
        mem_Free(w->objs);
        w->objs = 0;
    }
    if (w->flags & 0x1000)
        d2_BlitPart(0, 0, w->w, w->h, w->surf0, 0, 0, d2_BackSurf);
    if (w->surf0)
        d2_FreeSurface(w->surf0);
    if (w->surf1)
        d2_FreeSurface(w->surf1);
    if (w->surf2)
        d2_FreeSurface(w->surf2);
    if (w->spr)
        d3if_FreeSprite((int)w->spr);
    if (w->text) {
        mem_Free(w->text);
        w->text = 0;
    }
    if (w->scrollBar != -1)
        d2_RemoveObj(w->scrollBar);
    if (w->cornerObj != -1)
        d2_RemoveObj(w->cornerObj);
    if (w->extraObj != -1)
        d2_RemoveObj(w->extraObj);
    memset(w, 0, sizeof(D2WIN));
    w->id = win;
    w->bar = -1;
    d2_WindowsOpen--;
    return 1;
}
