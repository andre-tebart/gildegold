/* 0x40dcb0, rotates *obj by (*x, *y, *z) degrees relative to its angles over *time; 1, a NULL object is a
   script error */

int ocmd_RotateObjectRelative(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, o->pos.x, o->pos.y, o->pos.z, *x * OCMD_DEG + o->rotation.x,
                        *y * OCMD_DEG + o->rotation.y, *z * OCMD_DEG + o->rotation.z, *time, 0);
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_RotateObjectRelative: Could not find object to rotate...");
    return 1;
}
