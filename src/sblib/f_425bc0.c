/* 0x425bc0, opens the sound driver once and allocates the count sample slots (memory budget mem, default
   0x1fffcc bytes), zeroes them and initializes each through sfx_AllocSample, seeds rand with the time and
   prints the sblib message; side effects: sb_Driver / sb_Mem / sb_Count / sb_Samples. Returns 0, -1 when a
   sample slot failed to initialize, and -1 when the driver is already open or the allocation failed. */
int sb_Init(int mem, int count, int a, int b, int c)
{
    int ret = 0;
    int i;

    if (sb_Driver == 0) {
        sb_Dummy.info = &sb_DummyEntry;
        if (sfx_OpenDigOutput(&sb_Driver, a, b, c) == 0) {
            sb_Mem = mem;
            if (!mem)
                sb_Mem = 0x1fffcc;
            sb_Count = count;
            sb_Samples = mem_Alloc(count * sizeof(SBSAMPLE), "sb_Init");
            if (sb_Samples) {
                D3_ZERO_AT(sb_Samples, sb_Count * sizeof(SBSAMPLE));
                for (i = 0; i < sb_Count; i++)
                    if (sfx_AllocSample(sb_Driver, &sb_Samples[i]) == -1)
                        ret = -1;
                srand(time(NULL));
                sprintf(sb_Msg, "sblib successfully initialzied ! May use %d bytes of memory", mem);
                return ret;
            }
        }
    }
    return -1;
}
