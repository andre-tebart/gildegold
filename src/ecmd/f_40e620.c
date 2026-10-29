/* 0x40e620, sets the global d3_NoTextures flag; 1 */

int ocmd_SetNoTextures(int *on)
{
    d3_NoTextures = *on;
    return 1;
}
