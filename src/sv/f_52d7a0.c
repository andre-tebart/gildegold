/* 0x52d7a0, per-frame camera follow of the stored view (0x75fb80 / 0x75fb90): when the camera shows a different view than the stored one it is reset (0x433b10) or moved (0x528140); ends the highlight
   (0x6f22ec, via its +0x18c callback) and clears the selection (0x52adb0) when a group is selected */

void sv_CameraFollow(void)
{
    int fl, n;

    fl = 0;
    if (SV_I((unsigned)&d3t_CamMove) != 0) {
        if (d3_VecEqual((char *)(unsigned)&d3_Unk1466620, (char *)(unsigned)&ai_Unk75fb80, 0.1f) == 0) {
            d3anim_FreePath(ai_Unkfc2dec);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
        } else {
            d3anim_FreePath(ai_Unkfc2dec);
            SV_I((unsigned)&d3t_CamMove) = 0;
            SV_I((unsigned)&d3t_CamMoving) = 0;
            fl = 1;
            d3_SetObjectPos(ai_Unkfc2dec, (unsigned)&ai_Unk75fb80);
            d3_SetObjectRot(ai_Unkfc2dec, (unsigned)&ai_Unk75fb90);
        }
    }
    if (SV_I((unsigned)&sv_FrameDepth) <= SV_I((unsigned)&sv_FrameBase) || fl) {
        if ((d3_VecEqual(ai_Unkfc2dec + 0x84, (char *)(unsigned)&ai_Unk75fb90, 0.1f) == 0 || d3_VecEqual(ai_Unkfc2dec + 0x4c, (char *)(unsigned)&ai_Unk75fb80, 0.1f) == 0)
            && (!fl && (sv_Unk528140(), SV_I((unsigned)&full_data_6f22ec) != 0))) {
            n = *(int *)(SV_I((unsigned)&full_data_6f22ec) + 0x1d4);
            if (n != 0 && *(int *)(n + 0x18c) != 0)
                ((int (*)())*(int *)(n + 0x18c))(SV_I((unsigned)&full_data_6f22ec), 3, n + 400, 0);
            SV_I((unsigned)&full_data_6f22ec) = 0;
        }
    } else if (SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_PopRequest) <= SV_I((unsigned)&full_data_6f22a8)) {
        sv_Unk528140();
        SV_I((unsigned)&full_data_6f22a8) = SV_I((unsigned)&sv_FrameDepth) - SV_I((unsigned)&sv_PopRequest);
        if (SV_I((unsigned)&full_data_6f22ec) != 0) {
            n = *(int *)(SV_I((unsigned)&full_data_6f22ec) + 0x1d4);
            if (n != 0 && *(int *)(n + 0x18c) != 0)
                ((int (*)())*(int *)(n + 0x18c))(SV_I((unsigned)&full_data_6f22ec), 3, n + 400, 0);
            SV_I((unsigned)&full_data_6f22ec) = 0;
        }
        if (SV_I((unsigned)&ai_Unk75ec5c) == 0)
            return;
        sv_Unk52adb0();
    }
    if (SV_I((unsigned)&ai_Unk75ec5c) == 0)
        return;
    sv_Unk52adb0();
}
