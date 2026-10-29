/* 0x40dd90, switches the light recalculation on (*on) or off and invalidates the light cache; 1 */

int ocmd_LightCalc(int *on)
{
    d3_LightInvalidate((char)*on);
    return 1;
}
