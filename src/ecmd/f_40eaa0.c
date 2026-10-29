/* 0x40eaa0, deletes the rain particle system *rain (d3t_DeleteRain); 0 */

int ocmd_DeleteRain(void **rain)
{
    d3t_DeleteRain(*rain);
    return 0;
}
