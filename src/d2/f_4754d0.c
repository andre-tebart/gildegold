/* 0x4754d0, sets D2OBJ::clickDisabled of every object on the windows of a view */

void d2_EnableViewObjects(int view, int on)
{
    D2VIEW *v = &d2_Views[view];
    int i, j;

    if (v->active != 0) {
        for (i = 0; i < v->nwins; i++) {
            D2WIN *w = &d2_Wins[v->wins[i]];

            for (j = 0; j < w->nobjs; j++)
                d2_Objs[w->objs[j]].clickDisabled = (on == 0);
        }
        v->shown = on;
    }
}
