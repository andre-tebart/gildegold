/* 0x40db30, moves *obj to (*x, *y, *z) over *time, keeping its angles (d3t_PlaceObject); 0, a NULL object is a
   script error */

int ocmd_MoveObject(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, (float)*x, (float)*y, (float)*z, o->rotation.x, o->rotation.y, o->rotation.z, *time, 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_MoveObject: Could not find object to move...");
    return 0;
}
