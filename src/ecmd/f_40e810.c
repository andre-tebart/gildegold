/* 0x40e810, same without the error */

int ocmd_NewCameraFlight(int *time, char **a, char **b, char **c, char **d, char **e, char **f)
{
    if (evt_CurCall && evt_CurCall->fn == ocmd_NewCameraFlight) {
        if (d3t_CamMoving || d3t_CamMove) {
            evt_Cur->waitfn = ocmd_NewCameraFlight;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_NewCameraFlight;
    d3t_MoveCameraPath(*time / 14, 6, *a, *b, *c, *d, *e, *f);
    return 1;
}
