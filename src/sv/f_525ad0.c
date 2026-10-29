/* 0x525ad0, positions the sky for the time t (+4 hours, +6 minutes ..): the second of the day is mapped through the phase table 0x623baa .. 0x623bdc to 0 .. 600 (k); with the sky object 0x760628 its alpha (+0x26) is
   read; during a scene change (0xfd7c4c, until 0x76054c + 0x760548) the fade is 0x44a170 (0, 2, 1.0); when the phase hardly changed nothing happens; otherwise the weather day k % 7 is set (0x438d40 with the callback
   0x525a80), 0x623e94 = k / 100 - (k / 100 as int) and 0x449de0 (the day, the fraction, 1.0, changed) applies it. ROUGH */

void sv_Unk525ad0(char *t)
{
    int s, k, day, d, sc;
    float a;
    int changed;

    a = 0.0f;
    s = (*(int *)(t + 6) + *(unsigned short *)(t + 4) * 0x3c) * 0x3c;
    if (s < ai_Unk623baa[0])
        k = 0;
    else if (s < *(int *)((char *)ai_Unk623baa + 10))
        k = ((s - ai_Unk623baa[0]) * 100) / (*(int *)((char *)ai_Unk623baa + 10) - ai_Unk623baa[0]);
    else if (s < *(int *)((char *)ai_Unk623baa + 20))
        k = ((s - *(int *)((char *)ai_Unk623baa + 10)) * 100) / (*(int *)((char *)ai_Unk623baa + 20) - *(int *)((char *)ai_Unk623baa + 10)) + 100;
    else if (s < *(int *)((char *)ai_Unk623baa + 30))
        k = ((s - *(int *)((char *)ai_Unk623baa + 20)) * 200) / (*(int *)((char *)ai_Unk623baa + 30) - *(int *)((char *)ai_Unk623baa + 20)) + 200;
    else if (s < *(int *)((char *)ai_Unk623baa + 40))
        k = ((s - *(int *)((char *)ai_Unk623baa + 30)) * 100) / (*(int *)((char *)ai_Unk623baa + 40) - *(int *)((char *)ai_Unk623baa + 30)) + 400;
    else if (s < *(int *)((char *)ai_Unk623baa + 50))
        k = ((s - *(int *)((char *)ai_Unk623baa + 40)) * 100) / (*(int *)((char *)ai_Unk623baa + 50) - *(int *)((char *)ai_Unk623baa + 40)) + 500;
    else
        k = 600;
    if (ai_Unk760628 != 0)
        a = *(unsigned char *)(ai_Unk760628 + 0x26) * 0.003921569f;
    if (((int *)&ai_MapRefs)[d3_CurrentScene * 0xf8] != 0 && (unsigned)d3_Time < (unsigned)(ai_Unk76054c + ai_Unk760548)) {
        d3_SetFogSet(0, 2, 1.0f);
        ai_Unk6f2370 = 1;
        return;
    }
    if (ai_Unk6f2370 != 0) {
        d3_SetFogSet(0, 1, a);
        ai_Unk6f2370 = 0;
    }
    sc = d3_CurrentScene;
    if (((fabs(ai_Unk760420 - 1.0) < 0.1 && abs(ai_Unk760464 - k) < 10) && ai_Unk75ec20 == d3_CurrentScene) || d3t_CamMove != 0) {
        d3_SetFogSet(0, 1, a);
        return;
    }
    if (0.2 < fabs(ai_Unk760420 - 1.0) || abs(ai_Unk760464 - k) >= 100)
        changed = 1;
    else
        changed = ai_Unk75ec20 != d3_CurrentScene;
    ai_Unk75ec20 = d3_CurrentScene;
    ai_Unk760420 = 1.0f;
    ai_Unk760464 = k;
    day = (int)(k * 0.01f);
    d = day % 7;
    if (ai_Unk623e80 != d && ((int *)&ai_MapRefs)[sc * 0xf8] != 0) {
        if (d < 0 || ai_Unk6f2340 != 0) {
            if (d < 3 || ai_Unk6f2340 != 1)
                goto done;
            d3_ForEachObject(ai_Unk5fb5b0, 0, 6, sv_Unk525a80, 1);
        } else
            d3_ForEachObject(ai_Unk5fb5b0, 0, 6, sv_Unk525a80, 0);
        ai_Unk6f2340++;
        d = d;
    }
done:
    ai_Unk623e80 = d;
    ai_Unk623e94 = (float)k * 0.01f - (float)day;
    ai_Unk6f236c = d;
    d3_SetLightSet(d, ai_Unk623e94, 1.0f, changed);
    d3_SetFogSet(0, 1, a);
}
