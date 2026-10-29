/* 0x520ee0, registers sv_UpdateFramesPerTick as the tick callback (the period 0x47) and clears the selection flags
   sv_SelectionFlags (0xc0 words) and the bomb records cm_Bombs (0x40 words) */

void sv_InitTick(void)
{
    int i;

    tb_OpenFunction(sv_UpdateFramesPerTick, 0x47);
    for (i = 0; i < 0xc0; i++)
        sv_SelectionFlags[i] = 0;
    for (i = 0; i < 0x40; i++)
        cm_Bombs[i] = 0;
}
