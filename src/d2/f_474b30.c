/* 0x474b30, loads a view and makes it ready: its objects are hidden, the two images are created */

int d2_LoadView(int x, int y, char *name)
{
    int view, i, j;
    D2VIEW *v;
    D2WIN *w;
    int desc[16];

    view = d2_OpenForm(x, y, name);
    if (view != 0) {
    v = &d2_Views[view];
    for (i = 0; i < v->nwins; i++) {
        w = &d2_Wins[v->wins[i]];
        d2_Objs[w->parent].disabled = 1;
        for (j = 0; j < w->nobjs; j++)
            d2_Objs[w->objs[j]].disabled = 1;
    }
    v->loaded = 1;
    w = &d2_Wins[v->wins[0]];
    memset(desc, 0, sizeof(desc));
    desc[1] = w->w;
    desc[2] = w->h;
    v->viewSurf = (int)d2_MakeSurface(desc, 3);
    memset(desc, 0, sizeof(desc));
    desc[1] = w->w;
    desc[2] = w->h;
    v->texSurf = (int)d2_MakeSurface(desc, 3);
    return view;
    }
    return 0;
}
