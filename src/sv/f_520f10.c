/* 0x520f10, unregisters the tick callback */

void sv_UnregisterTick(void)
{
    tb_CloseFunction(sv_UpdateFramesPerTick);
}
