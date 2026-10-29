/* 0x46fad0, gives all buttons (type 9) of a window the width of the widest one plus 8 (at least 0x80) and
   centres them */

void d2_LayoutButtons(int win)
{
    D2WIN *w = &d2_Wins[win];
    D2OBJ *o;
    int i, max = 0;

    for (i = 0; i < w->nobjs; i++) {
        o = &d2_Objs[w->objs[i]];
        if (o->type == 9 && o->w > max)
            max = o->w;
    }
    max += 8;
    if (max < 0x80)
        max = 0x80;
    for (i = 0; i < w->nobjs; i++) {
        o = &d2_Objs[w->objs[i]];
        if (o->type == 9) {
            o->centred = 1;
            d2_SetObjWidth(w->objs[i], max);
        }
    }
}
