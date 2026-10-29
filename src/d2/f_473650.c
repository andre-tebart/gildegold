/* 0x473650, creates a text label on a window */

int d2_CreateWinText(short x, short y, char *text, int win)
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
    d2_Objs[obj].input = (int)mem_Alloc(0x101, "d2:ShowTxt");
    if ((char *)d2_Objs[obj].input == 0)
        return -1;
    strcpy((char *)d2_Objs[obj].input, text);
    if (w->font != 0)
        d2_Objs[obj].font = w->font;
    else
        d2_Objs[obj].font = d2_DefaultFont;
    d2_Objs[obj].type = 0x43;
    d2_Objs[obj].x = w->x + x;
    d2_Objs[obj].y = (w->y - w->scrolly) + y;
    d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font) + 0x60;
    d2_Objs[obj].h = d2_FontHeight;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].win = w;
    d2_Objs[obj].data = w;
    d2_Objs[obj].clipy0 = w->y;
    d2_Objs[obj].clipy1 = w->h + w->y;
    d2_Objs[obj].clipx0 = w->x;
    d2_Objs[obj].clipx1 = w->w + w->x;
    d2_Objs[obj].buf = d2_Objs[w->parent].buf;
    d2_Objs[obj].u.text[0] = 0;
    if (y + d2_Objs[obj].h > w->contentHeight)
        w->contentHeight = y + d2_Objs[obj].h;
    d2_AddToDrawOrder(obj, w);
    return w->objs[w->nobjs++] = obj;
}
