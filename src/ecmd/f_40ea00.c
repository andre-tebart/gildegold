/* 0x40ea00, zooms the camera onto the object *name over *time (d3t_PlaceAtObject), waiting while a camera flight
   runs; 1 */

int ocmd_ZoomOnObject(int *time, char **name)
{
    D3OBJECT *obj;

    if (evt_CurCall && evt_CurCall->fn == ocmd_ZoomOnObject) {
        if (d3t_CamMoving || d3t_CamMove) {
            evt_Cur->waitfn = ocmd_ZoomOnObject;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_ZoomOnObject;
    obj = d3_FindObject(0, 0x160, 0, *name);
    if (obj)
        d3t_PlaceAtObject(obj, *time / 17);
    return 1;
}
