/* 0x591480, the "near the build site" tick (150 B): in the '1' state with the camera 0xfc2dec, when the scene's
   current run function (the table 0x75f780, 8 bytes per entry, at 0x6f229c) is 0x591520, the event 0x576040
   (0x16) runs and the camera's target 0xfc2dec + 0x84 (the x / z, y = 0) is compared with 0.1; when the camera
   already sits there, 0x411860 (the frame?) moves it */

void ai_Unk591480(void)
{
    float v[3];

    if (U_I((unsigned)&kb_Last) == '1' && U_I((unsigned)&ai_Unkfc2dec) != 0 &&
        *(int *)(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (int)ai_GameFrame) {
        ai_Unk576040(0x16, 0, 0, 0);
        v[0] = *(float *)(U_I((unsigned)&ai_Unkfc2dec) + 0x84);
        v[1] = 0;
        v[2] = *(float *)(U_I((unsigned)&ai_Unkfc2dec) + 0x8c);
        if (d3_VecEqual((void *)(U_I((unsigned)&ai_Unkfc2dec) + 0x84), v, 0.1f) == 0)
            d3t_PlaceObjectV(U_I((unsigned)&ai_Unkfc2dec), U_I((unsigned)&ai_Unkfc2dec) + 0x4c, v, 0x1e, 0x28);
    }
}
