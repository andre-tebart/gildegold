/* 0x411c10, per frame: ends a finished camera move and keeps the sun rays on the camera. */

void d3t_UpdateCamera(void)
{
    if (d3t_CamMove) {
        d3t_CamMoving = 1;
        if (d3t_CamMove->flags2d & 0x20) {
            d3anim_FreePath(d3_Camera);
            d3t_CamMoving = 0;
            d3t_CamMove = 0;
        }
    }
    if (d3t_SunRays && d3_DefaultScene == d3t_SunRays->scene)
        d3t_UpdateSunRays(d3t_SunRays);
}
