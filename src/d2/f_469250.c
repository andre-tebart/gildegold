/* 0x469250, gives an object another picture */

void d2_SetObjPic(int obj, int pic)
{
    D2OBJ *o = &d2_Objs[obj];

    if (o->pic != pic) {
        if (o->type < 0x40)
            d2_SubRefcount(o->pic);
        o->data = d2_GetPic(pic);
        o->input = d2_PicShift;
        o->pic = d2_PicShift + pic;
        o->w = d2_Pics[pic].w;
        o->h = d2_Pics[pic].h;
        o->clipy0 = 0;
        o->clipy1 = d2_ScreenH;
        o->clipx0 = 0;
        o->clipx1 = d2_ScreenW;
        if (o->type < 0x40)
            d2_AddRefcount(o->pic);
    }
}
