/* 0x527e90, like 0x527840 for the model m directly (no object): moves the camera animated (0x411860 with 0x68 steps) 600 units back from the model */

void sv_Unk527e90(int m)
{
    float lo, hi, k, h;
    float pos[3], dir[3], cen[3], back[3], rot[16];   /* rot is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    double t;

    d3t_GetHeightRange(m, &lo, &hi);
    d3_MatTranspose(ai_Unkfc2dec + 0x18c, rot);
    back[0] = rot[0] * -0.0f + rot[3] * -0.0f + rot[6] * -600.0f;
    back[1] = rot[1] * -0.0f + rot[4] * -0.0f + rot[7] * -600.0f;
    back[2] = rot[2] * -0.0f + rot[5] * -0.0f + rot[8] * -600.0f;
    d3_PointToWorld(m, m + 0x4c, cen);
    pos[0] = cen[0] + back[0];
    pos[1] = cen[1] + back[1];
    pos[2] = cen[2] + back[2];
    t = d3t_GetMeanHeight(pos);
    pos[1] = (float)((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t);
    h = pos[1] - cen[1];
    d3t_GetHeightRange(m, &lo, &hi);
    k = back[1] / ((hi - lo) * 0.5f + h);
    pos[0] = pos[0] - (k * back[0] - back[0] * 1.8f);
    pos[2] = pos[2] - (k * back[2] - back[2] * 1.8f);
    dir[0] = *(float *)(ai_Unkfc2dec + 0x84);
    dir[1] = *(float *)(ai_Unkfc2dec + 0x88);
    dir[2] = *(float *)(ai_Unkfc2dec + 0x8c);
    d3anim_FreePath(ai_Unkfc2dec);
    d3t_CamMoveI = 0;
    ai_Unk6310e4 = 0;
    t = d3t_GetMeanHeight(pos);
    pos[1] = (float)((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t);
    d3t_PlaceObjectV(ai_Unkfc2dec, pos, dir, 0x68);
}
