/* 0x43e680 */

void d3_Frame(unsigned int mask, char flag, char tick)
{
    d3_Render(mask, flag);
    d3_Animate(mask, flag, tick, 1);
}
