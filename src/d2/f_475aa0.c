/* 0x475aa0, marks an object and everything on it as disabled (on = 0) or enabled */

void d2_EnableObject(int obj, int on)
{
    D2OBJ *o = &d2_Objs[obj];
    D2WIN *w;
    int i;

    o->disabled = (on == 0);
    if (o->type == 0x40) {
        w = &d2_Wins[o->input];
        for (i = 0; i < w->nobjs; i++)
            d2_EnableObject(w->objs[i], on);
        if (w->scrollBar != -1)
            d2_EnableObject(w->scrollBar, on);
        if (w->cornerObj != -1)
            d2_EnableObject(w->cornerObj, on);
        if (w->extraObj != -1)
            d2_EnableObject(w->extraObj, on);
    }
}
