/* 0x43cbe0, makes obj the active camera (d3_Camera) when it is a camera object (type 3) and is not the
   current one; the default scene is then rebuilt through d3_UpdateAll(0) (not the full variant). */

void d3_SetCamera(D3OBJECT *obj)
{
    if (obj != d3_Camera && obj->type == 3) {
        d3_Camera = obj;
        d3_UpdateAll(0);
    }
}
