/* 0x520ec0, the tick callback: turns the frame counter sv_TickFrames into the float sv_FramesPerTick and clears it */

void sv_UpdateFramesPerTick(void)
{
    sv_FramesPerTick = (float)sv_TickFrames;
    sv_TickFrames = 0;
}
