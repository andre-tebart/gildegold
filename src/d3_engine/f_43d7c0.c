/* 0x43d7c0, switches the object to the surface of its current level of detail, transforms it with the
   mesh driver and marks the object dirty. */

void d3_TransformSurf(D3OBJECT *obj)
{
    D3OBJINFO *info;
    char buf[0x40];

    info = d3_SelectSurf(obj);
    if (info) {
        if (info != obj->info)
            obj->info = info;
        d3_WorldMatrixD(obj, (obj->state & 0x80) ? 0 : d3_Camera, 0, buf);
        D3_OPS(info)->Transform(obj, info, buf);
        obj->state |= 4;
    }
}
