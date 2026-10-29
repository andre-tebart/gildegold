/* 0x527840, focuses the camera on the object o (+0x61 its model): the camera is put 600 units back along the view direction from the object's door / centre (0x458a20, the terrain height 0x412550 + the height blend),
   perspective-corrected by the model's extent (0x411c70), moved there with the pitch 3 * 0x623814 and 0x68 steps (0x411860), the position stored in the player's camera record (0x506430) and the object noted
   (0x75ec60 / 0x6f2304). ROUGH */

void sv_Unk527840(int o)
{
    int m;
    float lo, hi;
    float pos[3], dir[3], cen[3], back[3], rot[16];   /* rot is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    float k, h;
    double t;
    char *rec;

    m = *(int *)(o + 0x61);
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
    d3t_CamMove = 0;
    ai_Unk6310e4 = 0;
    t = d3t_GetMeanHeight(pos);
    pos[1] = (float)((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + t);
    d3t_PlaceObjectV(ai_Unkfc2dec, pos, dir, (int)(float)(ai_CameraStep * 3), 0x68);
    ai_Unk75fb90 = *(int *)&dir[0];
    ai_Unk75fb80 = *(int *)&pos[0];
    ai_Unk75fb94 = *(int *)&dir[1];
    ai_Unk75fb84 = *(int *)&pos[1];
    ai_Unk75fb98 = *(int *)&dir[2];
    ai_Unk75fb88 = *(int *)&pos[2];
    rec = gm_PlayerRecord(ai_Chars[ai_PlayerId]);
    if (rec != 0) {
        *(float *)(rec + 0x8c) = pos[0];
        *(float *)(rec + 0x90) = pos[1];
        *(float *)(rec + 0x94) = pos[2];
        *(int *)(rec + 0x9c) = *(int *)&dir[0];
        *(int *)(rec + 0xa0) = *(int *)&dir[1];
        *(int *)(rec + 0xa4) = *(int *)&dir[2];
    }
    ai_Unk75ec60 = o;
    ai_Unk6f2304 = *(int *)(o + 0x61);
}
