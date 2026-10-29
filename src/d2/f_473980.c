/* 0x473980, creates a caption object on a window */

int d2_CreateWinCaption(short x, short y, char *text, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj;

    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_NewObject();
    strcpy(&d2_Objs[obj].u.text[4], text);
    if (w->font != 0)
        d2_Objs[obj].font = w->font;
    else
        d2_Objs[obj].font = d2_DefaultFont;
    d2_Objs[obj].type = 0x46;
    d2_Objs[obj].x = w->x + x;
    d2_Objs[obj].y = (w->y - w->scrolly) + y;
    d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font) + 0x60;
    d2_Objs[obj].h = d2_FontHeight;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].win = w;
    d2_Objs[obj].data = w;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    d2_Objs[obj].buf = d2_Objs[w->parent].buf;
    d2_AddToDrawOrder(obj, w);
    w->objs[w->nobjs] = obj;
    w->nobjs++;
    return obj;
}
