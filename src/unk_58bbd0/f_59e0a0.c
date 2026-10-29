/* 0x59e0a0, switches the current building room (the record 0x6f2308 + 0x29) to the param_1-th entry of the
   building 0x4ffe90 (0x6f2308 + 0x5d, 1, 4, 2), or to the word param_2 when param_1 == -1; 0x6f22a0 = the
   0x6f229c - 0x6f22a4 + 1 count, the room matrices 0x43b790 / 0x43b860 (0xfc2dec + 0 / + 0x10), the counters
   0x6f22cc / 0x6f22d0 = 0 and the sound "RaumWechseln_s" (0x426fa0) */

void ai_Unk59e0a0(int param_1, unsigned short param_2)
{
    unsigned short *p;
    int i;

    if (param_1 == -1) {
        *(unsigned short *)(U_I((unsigned)&ai_PlayerBuilding) + 0x29) = param_2;
    } else {
        p = (unsigned short *)gm_Query(U_I(U_I((unsigned)&ai_PlayerBuilding) + 0x5d), 1, 4, 2);
        if (p == 0)
            return;
        for (i = 1; i < param_1; i++) {
            p = (unsigned short *)gm_QueryNext();
            if (p == 0)
                return;
        }
        *(unsigned short *)(U_I((unsigned)&ai_PlayerBuilding) + 0x29) = *p;
    }
    U_I((unsigned)&sv_PopRequest) = (U_I((unsigned)&sv_FrameDepth) - U_I((unsigned)&sv_FrameBase)) + 1;
    d3_SetObjectPos((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x10));
    d3_SetObjectRot((void *)U_I((unsigned)&ai_Unkfc2dec), (void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x20));
    U_I((unsigned)&ai_Unk6f22cc) = 0;
    U_I((unsigned)&ai_Unk6f22d0) = 0;
    sb_PlaySample(U_I((unsigned)&full_data_6f2410), "RaumWechseln_s", 1, 0x3f, 0x7f, 0, 0);
}
