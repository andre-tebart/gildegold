/* 0x525ee0, eases the camera height (+0x50) toward the wanted one (the terrain height + 0x623800 .. 0x623808 blend by 0x6f22d4, minus the camera offset +0x7c): a difference above 5.0 is closed by 1/15 per call, at most 10.0;
   the step is kept in 0x6f2374 (not applied while the cutscene flag 0x6310e8 is set) */

void sv_Unk525ee0(void)
{
    double d;
    float step, pos[3];

    if (ai_Unkfc2dec == 0)
        return;
    d = d3t_GetMeanHeight(ai_Unkfc2dec + 0x4c);
    d = ((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + d) - (*(float *)(ai_Unkfc2dec + 0x7c) + *(float *)(ai_Unkfc2dec + 0x50));
    if (5.0 < fabs(d)) {
        d = d * 0.06666667;
        step = (float)d;
        if (d <= 0.0) {
            if (step < 0.0f && step <= -10.0f)
                step = -10.0f;
        } else if (10.0f <= step)
            step = 10.0f;
        pos[0] = *(float *)(ai_Unkfc2dec + 0x4c);
        pos[2] = *(float *)(ai_Unkfc2dec + 0x54);
        pos[1] = *(float *)(ai_Unkfc2dec + 0x50) + step;
        if (d3t_CamMoveI == 0)
            d3_SetObjectPos(ai_Unkfc2dec, pos);
        ai_Unk6f2374 = step;
        return;
    }
    ai_Unk6f2374 = 0.0f;
}
