/* 0x40e670, switches the fog to the blend of fog sets *on and *a, *b in percent; 1 */

int ocmd_SetFogSet(int *on, int *a, int *b)
{
    d3_SetFogSet(*on, *a, max(min(*b * 0.01f, 1.0f), 0.0f));
    return 1;
}
