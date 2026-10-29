/* 0x475600, centres the windows of a view */

void d2_CentreViewWindows(int view)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    for (i = 0; i < v->nwins; i++) {
        D2WIN *w = &d2_Wins[v->wins[i]];
        if (d2_Objs[w->parent].win == 0)
            d2_MoveObj(d2_HalfScreenW - w->w / 2, d2_HalfScreenH - w->h / 2, w->parent);
    }
}
