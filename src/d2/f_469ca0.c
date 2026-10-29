/* 0x469ca0, opens a number input with an icon */

int d2_OpenInputIcon(int x, int y, int icon, unsigned char flags)
{
    int i;
    int obj;
    D2INPUT *in;
    D2PIC *ic;
    int sh;
    int n, v;
    char fmt[8];
    char buf[16];

    i = 1;
    if (d2_Inputs[1].used) {
        while (d2_Inputs[++i].used)
            ;
        if (i > 0x7f) {
            err_Message("d2_OpenInputIcon(): Too many inputs on screen...");
            return -1;
        }
    }
    in = &d2_Inputs[i];
    in->x = x;
    in->y = y;
    in->icon = icon;
    in->id = i;
    ic = &d2_Pics[icon];
    in->w = ic->w;
    in->h = ic->h;
    in->minval = 0;
    in->value = 0;
    in->flags = flags | 0x100;
    in->maxval = 999;
    in->ndigits = 3;
    in->used = 1;
    in->font = d2_FindPic("_FONT");
    d2_AddRefcount(icon);
    if (flags & 2) {
        n = 0;
        v = in->maxval;
        do {
            if (v <= 0)
                break;
            v /= 10;
            n++;
        } while (n < 10);
        in->ndigits = n;
        sprintf(fmt, "%%0%ii", in->ndigits);
        sprintf(buf, fmt, in->mult * in->value);
    }
    if (flags & 0x80) {
        in->maxval = max(in->maxval, 0x19);
    }
    obj = d2_NewObject();
    d2_Objs[obj].type = 0x41;
    d2_Objs[obj].x = in->x;
    d2_Objs[obj].y = in->y;
    d2_Objs[obj].w = in->w;
    d2_Objs[obj].h = in->h;
    d2_Objs[obj].pic = icon;
    d2_Objs[obj].input = i;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    d2_Objs[obj].data = in;
    d2_Objs[obj].font = d2_DefaultFont;
    in->obj = obj;
    return obj;
}
