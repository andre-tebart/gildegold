/* 0x45db20 */

void d3sky_Free(void)
{
    if (d3_Sky) {
        d3sky_Destroy(d3_Sky);
        d3_Sky = 0;
    }
}
