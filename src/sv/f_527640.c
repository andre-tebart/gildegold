/* 0x527640, the per-frame camera update: with a camera and no cutscene (0x6333e0, 0x760440, 0x62a8cc = -1) 0x5263a0 (when no mouse button), the mouse control 0x526cf0 (unless flag 0x8000 of 0x760638), the
   height easing 0x525ee0 (option 0x62381c) and, when the camera moved without a selection, the stored camera record of the player (0x506430: position, direction, zoom); returns 0x6f22b4; without a camera it
   restores the clip window to the screen (0x5f3238 / 0x5f323c, 0x4113f0 (mouse x, mouse x)) once */

int sv_Unk527640(void)
{
    char *rec;

    if (ai_Unkfc2dec != 0 && tb_Paused == 0) {
        if (ai_Unk760440 == 0 && ai_Unk62a8cc == -1) {
            if (dm_L.rhold == 0)
                ai_Unk6f22b4 = sv_Unk5263a0();
            if (-1 < (char)(sv_FrameMask >> 8))
                sv_Unk526cf0();
            if (ai_Unk62381c != 0 && kb_Down[0xf] == 0)
                sv_Unk525ee0();
            if (ai_Unk6f22b4 != 0 && sv_FrameDepth == sv_FrameBase && ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0
                && (rec = gm_PlayerRecord(ai_Chars[ai_PlayerId])) != 0) {
                CAMLOG(rec);
                *(float *)(rec + 0x88) = sv_CameraAngleFactor;
            }
            ai_Unk75fb90 = *(int *)(ai_Unkfc2dec + 0x84);
            ai_Unk75fb94 = *(int *)(ai_Unkfc2dec + 0x88);
            ai_Unk75fb98 = *(int *)(ai_Unkfc2dec + 0x8c);
            ai_Unk75fb80 = *(int *)(ai_Unkfc2dec + 0x4c);
            ai_Unk75fb84 = *(int *)(ai_Unkfc2dec + 0x50);
            ai_Unk75fb88 = *(int *)(ai_Unkfc2dec + 0x54);
        }
        return ai_Unk6f22b4;
    }
    ai_Unk6f22b4 = 0;
    if (di_Absolute == 0) {
        di_MaxY = (int)d2_ScreenH;
        di_MaxX = d2_ScreenW - 8;
        di_MinX = 0;
        di_MinY = 0;
        di_Absolute = 1;
        di_SetMousePos((int)dm_L.x, (int)dm_L.x);
    }
    return 0;
}
