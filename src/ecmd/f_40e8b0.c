/* 0x40e8b0, flights *obj along the path of up to five dummies *a..*e over *time; waits while a camera flight
   runs, a script error when *obj is NULL; 1 */

int ocmd_ObjectFlight(D3OBJECT **obj, int *time, char **a, char **b, char **c, char **d, char **e)
{
    D3OBJECT *path[5];
    int i, n;

    n = 0;
    if (evt_CurCall && evt_CurCall->fn == ocmd_ObjectFlight) {
        if (d3t_CamMoving || d3t_CamMove) {
            evt_Cur->waitfn = ocmd_ObjectFlight;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_ObjectFlight;
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "ObjectFlight(): Could not find object");
        return 0;
    }
    path[0] = d3_FindObject(0, 0x140, 0, *a);
    path[1] = d3_FindObject(0, 0x140, 0, *b);
    path[2] = d3_FindObject(0, 0x140, 0, *c);
    path[3] = d3_FindObject(0, 0x140, 0, *d);
    path[4] = d3_FindObject(0, 0x140, 0, *e);
    for (i = 0; i < 5; i++)
        if (path[i])
            n++;
    d3t_MoveObjectPath(*obj, *time / 14, n, path);
    return 1;
}
