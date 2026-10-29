/* 0x40e6f0, starts a camera flight through the two dummies *a / *b over *time (17 units per second); a script
   error when a dummy is missing; 1 */

int ocmd_CameraFlight(int *time, char **a, char **b)
{
    if (!d3t_MoveCameraPath(*time / 17, 2, *a, *b))
        evt_Error(evt_Cur, evt_Cur->pc, "CameraFlight(): Could not find one or few dummies");
    return 1;
}
