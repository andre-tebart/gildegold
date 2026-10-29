/* 0x40dd50, sets the ambient light colour from *r / *g / *b and invalidates the light cache; 1 */

int ocmd_SetAmbiente(int *r, int *g, int *b)
{
    d3if_AmbR = (float)*r;
    d3if_AmbG = (float)*g;
    d3if_AmbB = (float)*b;
    d3_LightInvalidate(1);
    return 1;
}
