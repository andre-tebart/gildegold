/* 0x476710, clears the paintbox of the current window */

void d2_ClearPaintbox(void)
{
    D2PAINTWIN *w = WIN(d2_LastWindow);

    if (w->active == 0) {
        err_Message("d2_ClearPaintbox:Invalidate window!");
        return;
    }
    if (w->paint == 0) {
        err_Message("d2_ClearPaintbox:Window has no paintbox!");
        return;
    }
    d2_ClearSurface(w->paint);
}
