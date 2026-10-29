/* 0x40dc30, moves *obj by (*x, *y, *z) relative to its position over *time; 0, a NULL object is a script error */

int ocmd_MoveObjectRelative(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, *x + o->pos.x, *y + o->pos.y, *z + o->pos.z,
                        o->rotation.x, o->rotation.y, o->rotation.z, *time, 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_MoveObjectRelative: Could not find object to move...");
    return 0;
}
