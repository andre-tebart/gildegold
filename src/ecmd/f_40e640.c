/* 0x40e640, switches the day-time light set of d3_interface.c to *set (no blend); 1 */

int ocmd_SetLightSet(int *set)
{
    d3_SetLightSet(*set, 1.0f, 1.0f, 0);
    return 1;
}
