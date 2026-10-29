/* 0x40e750, waits until the flight is over */

int ocmd_CameraFlightEnhanced(int *time, char **a, char **b, char **c, char **d, char **e, char **f)
{
    if (evt_CurCall && evt_CurCall->fn == ocmd_CameraFlightEnhanced) {
        if (d3t_CamMoving || d3t_CamMove) {
            evt_Cur->waitfn = ocmd_CameraFlightEnhanced;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_CameraFlightEnhanced;
    if (!d3t_MoveCameraPath(*time / 17, 6, *a, *b, *c, *d, *e, *f))
        evt_Error(evt_Cur, evt_Cur->pc, "CameraFlight(): Could not find one or few dummies");
    return 1;
}
