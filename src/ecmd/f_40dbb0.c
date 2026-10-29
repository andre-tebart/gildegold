/* 0x40dbb0, returns its first argument */

int ocmd_RotateObject(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, o->pos.x, o->pos.y, o->pos.z,
                        *x * OCMD_DEG, *y * OCMD_DEG, *z * OCMD_DEG, *time, 0);
        return (int)obj;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_RotateObject: Could not find object to rotate...");
    return (int)obj;
}
