/* 0x4730e0, puts an object into the draw order list just above its window */

void d2_AddToDrawOrder(int obj, D2WIN *win)
{
    D2OBJ *p;
    int i;

    i = 0;
    do {
        if (d2_DrawOrder[i] == &d2_Objs[obj]) {
            if (i == 0x2ff)
                d2_DrawOrder[0x2ff] = 0;
            else
                memmove(&d2_DrawOrder[i], &d2_DrawOrder[i + 1], 0xbf8 - i * 4);
            break;
        }
        i++;
    } while (i < 0x2ff);
    i = 0x2ff;
    while ((p = d2_DrawOrder[i], p == 0 || ((p->win != win || p->type == 0x40) && p->id != win->parent))) {
        i--;
        if (i < 0) {
            err_Message("Toby, wir haben ein Problem...");
            return;
        }
    }
    if (i >= 0) {
        memmove(&d2_DrawOrder[i + 2], &d2_DrawOrder[i + 1], 0xbf8 - i * 4);
        d2_DrawOrder[i + 1] = &d2_Objs[obj];
        return;
    }
    err_Message("Toby, wir haben ein Problem...");
}
