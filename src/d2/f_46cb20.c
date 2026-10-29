/* 0x46cb20, creates an object showing a picture at x, y */

int d2_OpenPic(x, y, pic)
int x;
int y;
int pic;
{
    int n;
    D2OBJ *o;
    void *data = 0;
    int real, p;
    D2PICHDR *set;
    D2PICHDR11 *set17;
    D2FRAME *spr;

    if (pic > d2_NPics)
        return -1;
    n = d2_NewObject();
    o = &d2_Objs[n];
    if (n >= 0x300)
        return -1;
    d2_GetPicEx(pic, &data, &real);
    o->data = data;
    p = real;
    o->pic = p;
    o->type = d2_Pics[p].type;
    o->x = (short)x;
    o->y = (short)y;
    o->clipx0 = (short)d2_ClipLeft;
    o->clipx1 = (short)d2_ClipRight;
    o->clipy0 = (short)d2_ClipTop;
    o->clipy1 = (short)d2_ClipBottom;
    o->drawFrames = 2;
    d2_AddRefcount(p);
    if (o->type == 8)
        o->interactive = 1;
    o->icon = -1;
    o->iconx = 0;
    o->icony = 0;
    switch (o->type) {
    case 1:
        set = (D2PICHDR *)d2_Objs[n].data;
        o->w = set->w;
        o->h = set->h;
        return n;
    case 4:
        d2_Objs[n].input = d2_StartAnim(x, y, d2_Objs[n].data, 0);
        set = (D2PICHDR *)d2_Objs[n].data;
        d2_Objs[n].w = set->w;
        d2_Objs[n].h = set->h;
        return n;
    case 0x11:
        set17 = (D2PICHDR11 *)d2_Objs[n].data;
        d2_Objs[n].w = set17->w;
        d2_Objs[n].h = set17->h;
        d2_Objs[n].input = 2;
        return n;
    case 5:
    case 8:
        o->input = d2_PicShift;
        set = (D2PICHDR *)d2_Objs[n].data;
        spr = d2_GetFrame(set, (short)d2_Objs[n].input);
        o->w = spr->w;
        o->h = spr->h;
        o->captionX = -1;
        o->captionY = -1;
        o->picW = set->w;
        o->picH = set->h;
        d2_Objs[n].shadowX = 8;
        d2_Objs[n].shadowY = 8;
        break;
    }
    return n;
}
