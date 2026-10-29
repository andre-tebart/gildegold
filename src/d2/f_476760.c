/* 0x476760, fills a rectangle of the paintbox of the current window */

void d2_FillPaintbox(int x, int y, int w, int h)
{
    D2PAINTWIN *win = WIN(d2_LastWindow);

    if (win->active == 0) {
        err_Message("d2_ClearPaintbox:Invalidate window!");
        return;
    }
    if (win->paint == 0) {
        err_Message("d2_ClearPaintbox:Window has no paintbox!");
        return;
    }
    d2_ClearRect(x, y, w, h, win->paint);
}
