/* 0x43fed0, frees the texture cache and resets the detail scale */

void d3fl_Exit(void)
{
    d3fl_FreeCache();
    d3fl_DetailScale = 0;
}
