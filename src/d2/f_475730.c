/* 0x475730, the same with float coordinates, kept on the screen */

void d2_PlaceWindowScaled(win, fl)
int win;
int fl;
{
    D2WIN *w = &d2_Wins[win];
    float x, y;
    int h, h2;
    unsigned char flags = (unsigned char)fl;

    if (flags & 1) {
        h = w->w / 2;
        x = (float)(w->x + h) - d2_HalfScreenW / d2_ScreenScale + d2_HalfScreenW - h;
    } else {
        x = w->x;
    }
    if (flags & 2) {
        h2 = w->h / 2;
        y = (float)(w->y + h2) * d2_ScreenScale - h2;
    } else {
        y = w->y;
    }
    if ((float)(d2_ScreenW - w->w) < x)
        x = (float)(d2_ScreenW - w->w);
    y = min((float)(d2_ScreenH - w->h), y);
    d2_MoveObj((int)x, (int)y, w->parent);
}
