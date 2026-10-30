/* 0x40eea0, stops the camera flight: frees the camera's path and clears the moving flags; 0 */

int ocmd_StopCameraFlight(void)
{
    d3anim_FreePath(d3_Camera);
    d3t_CamMoving = d3t_CamMoveI = 0;
    return 0;
}
