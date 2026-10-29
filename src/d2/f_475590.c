/* 0x475590, enables / disables the objects of a view */

void d2_SetViewEnabled(int view, int on)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    if (v->active) {
        for (i = 0; i < 0x300; i++) {
            if (d2_Objs[i].type != 0 && d2_Objs[i].buf == (D2CTRL *)v)
                d2_EnableObject(i, on);
        }
        v->enabled = on;
    }
}
