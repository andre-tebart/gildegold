/* 0x475870, places every window of a view that is not a child window, flags as in d2_PlaceWindow */

void d2_PlaceViewWindows(int view, unsigned char flags)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    for (i = 0; i < v->nwins; i++) {
        if (d2_Objs[d2_Wins[v->wins[i]].parent].win == 0)
            d2_PlaceWindow(v->wins[i], flags);
    }
}
