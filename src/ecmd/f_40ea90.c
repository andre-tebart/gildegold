/* 0x40ea90, creates a rain particle system of *count drops (d3t_CreateRain); returns it */

int ocmd_CreateRain(int *count)
{
    return (int)d3t_CreateRain(*count);
}
