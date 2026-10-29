/* 0x46c3f0, copies the pixels of a raw object to the surface (clipped at the top and bottom) */

void d2_DrawRaw(int obj, D2SURF *surf)
{
    D2OBJ *o = &d2_Objs[obj];
    int i;
    int top = 0;
    int h = o->h;

    if (o->y + h > o->clipy1)
        h = o->clipy1 - o->y;
    else if (o->y < o->clipy0)
        top = o->clipy0 - o->y;
    for (i = top; i < h; i++)
        memcpy(surf->bits + (o->y + i) * surf->pitch + o->x, o->u.raw + o->w * i, o->w * 2);
}
