/* 0x526cf0, the camera control by the mouse (buttons 0x14673a8 / 0x1467390, position 0x14683c0 / 2, wheel 0x14673c0 / c4): while a button is held the clip window is opened to the whole screen (0x5f3238 / 0x5f323c =
   32000), the first press stores the start (0x6f1c88 / 0x6f1c48 ..); both buttons: zoom / rotate by the drag (0x6f22d4 / d8 + the mouse delta * 0.0015, the height from 0x623800 .. 0x623808, the angle from 0x6f23b0 -
   0.0035 * dx), the camera is moved (0x43b790 / 0x43b860) unless it hardly differs, and the player's camera record (0x506430) updated (+0x88 .. +0xa4); the wheel changes the zoom by 0.1 per notch (0x525dd0); the left
   button alone drags the map (the delta rotated by the view, 0x6f3614 * 0.0606, clamped to the map rectangle); releasing restores the clip window (0x4113f0). Returns the activity count. ROUGH */

extern int ai_Unk6f23b8, ai_Unk6f23a0, ai_Unk6f23a4, ai_Unk6f2398, ai_Unk6f239c, ai_Unk6f1c88, ai_Unk6f1c48, ai_Unk6f1c50, ai_Unk6f1c40, ai_Unk6f1c84, ai_Unk6f2298, ai_Unk6f2290, ai_Unk6f1c8c;  /* 0x6f23b8, 0x6f23a0, 0x6f23a4, 0x6f2398, 0x6f239c, 0x6f1c88, 0x6f1c48, 0x6f1c50, 0x6f1c40, 0x6f1c84, 0x6f2298, 0x6f2290, 0x6f1c8c */
extern int ai_OptionScrollSpeed;
extern float ai_Unk6f23a8, ai_Unk6f23ac, ai_Unk6f23b0, ai_Unk6f23b4, ai_Unk6f1c64[3];  /* 0x6f23a8, 0x6f23ac, 0x6f23b0, 0x6f23b4, 0x6f1c64 */
extern int tb_Paused, ai_Unk760440, ai_Unk62a8cc, ai_Unk6f22b4, sv_FrameDepth, sv_FrameBase, ai_Unk75ec60, ai_Unk6f2304;  /* 0x6333e0, 0x760440, 0x62a8cc, 0x6f22b4, 0x6f229c, 0x6f22a4, 0x75ec60, 0x6f2304 */
extern char ai_Unk62381c;
int d3t_GetHeightRange();                     /* 0x411c70 */
short * gm_PlayerRecord(char *ch);                   /* 0x506430 */
void sv_PlaceCamera(float f);
void sv_Unk525ee0(void);
int sv_Unk5263a0(void);
int sv_Unk526cf0(void);
#define CAMLOG(r) do { \
        *(int *)((r) + 0x8c) = *(int *)(ai_Unkfc2dec + 0x4c); \
        *(int *)((r) + 0x90) = *(int *)(ai_Unkfc2dec + 0x50); \
        *(int *)((r) + 0x94) = *(int *)(ai_Unkfc2dec + 0x54); \
        *(int *)((r) + 0x9c) = *(int *)(ai_Unkfc2dec + 0x84); \
        *(int *)((r) + 0xa0) = *(int *)(ai_Unkfc2dec + 0x88); \
        *(int *)((r) + 0xa4) = *(int *)(ai_Unkfc2dec + 0x8c); \
    } while (0)

int sv_Unk526cf0(void)
{
    short my;
    char *cam;
    char *rec;
    float z, f2, ang, sn, cs;
    float pos[3], dst[3], dir[3], dv[3], right[16], fwd[3], v[2];   /* right is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    double dz;
    float *m;
    float lo, hi;
    int i;

    my = dm_P.y;
    cam = ai_Unkfc2dec;
    if (ai_Unk6310e4 != 0)
        return 0;
    if (dm_L.rhold == 0) {
    chk:
        if (dm_L.ldown != 0 && dm_L.rhold != 0)
            goto go;
    } else {
        if (ai_Unk6f23b8 == 0) {
            ai_Unk6f1c88 = (int)dm_L.x;
            ai_Unk6f1c48 = (int)dm_L.y;
            ai_Unk6f1c50 = di_MaxX;
            ai_Unk6f23b8 = 1;
            di_Absolute = 0;
            ai_Unk6f1c40 = di_MaxY;
            ai_Unk6f1c84 = di_MinX;
            ai_Unk6f2298 = di_MinY;
            di_MaxX = 32000;
            di_MaxY = 32000;
            di_MinX = -32000;
            di_MinY = -32000;
            ai_Unk6f2398 = ai_Unk6f1c88;
            ai_Unk6f239c = ai_Unk6f1c48;
        }
        if (dm_L.ldown != 0) {
            if (ai_Unk6f23a0 == 0) {
                ai_Unk6f23a0 = 1;
                ai_Unk6f2290 = (int)dm_P.x;
                ai_Unk6f23a8 = sv_CameraHeightFactor;
                ai_Unk6f1c8c = (int)dm_P.y;
                ai_Unk6f23b4 = *(float *)(ai_Unkfc2dec + 0x50) - ((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraAngleNear);
                ai_Unk6f23ac = sv_CameraAngleFactor;
                di_MaxY = ai_Unk6f1c8c - (int)(ai_Unk6f1c8c * 0.0f + 0);
                di_MinY = ai_Unk6f1c8c - (int)(ai_Unk6f1c8c * 0.0f + 0);
                ai_Unk6f23b0 = *(float *)(cam + 0x88);
            }
            goto chk;
        }
    }
    if (ai_Unk6f23a0 != 0) {
        ai_Unk6f23a0 = 0;
        ai_Unk6f1c48 = (int)my;
        di_MaxX = 32000;
        di_MaxY = 32000;
        ai_Unk6f1c88 = (int)dm_P.x;
        di_MinX = -32000;
        di_MinY = -32000;
        ai_Unk6f23a8 = (float)(ai_Unk6f1c48 - ai_Unk6f1c8c) * 0.004f + ai_Unk6f23a8;
    }
    if (dm_L.rhold == 0 && ai_Unk6f23b8 != 0) {
        di_MaxY = ai_Unk6f1c40;
        di_MinX = ai_Unk6f1c84;
        di_MaxX = ai_Unk6f1c50;
        di_MinY = ai_Unk6f2298;
        di_Absolute = 1;
        ai_Unk6f23b8 = 0;
        di_SetMousePos(ai_Unk6f2398, ai_Unk6f239c);
        ai_Unk6f2398 = 0;
        ai_Unk6f239c = 0;
        ai_Unk6f23a0 = 0;
        return 0;
    }
go:
    cam = ai_Unkfc2dec;
    if (cam != 0) {
        if (dm_L.rhold != 0 && dm_L.ldown != 0 && ai_Unk6f23a0 != 0) {
            f2 = (((float)(dm_L.wup - dm_L.wdown) * 5.0f + (float)(int)my) - (float)ai_Unk6f1c8c) * 0.0015f;
            sv_CameraHeightFactor = ai_Unk6f23a8 + f2;
            pos[0] = *(float *)(cam + 0x4c);
            pos[2] = *(float *)(cam + 0x54);
            pos[1] = (sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + ai_Unk6f23b4;
            sv_CameraAngleFactor = f2 + ai_Unk6f23ac;
            dir[2] = *(float *)(cam + 0x8c);
            dir[0] = (sv_CameraAngleFar - sv_CameraAngleNear) * sv_CameraAngleFactor + sv_CameraAngleNear;
            dir[1] = ai_Unk6f23b0 - (float)(dm_P.x - ai_Unk6f2290) * 0.0035f;
            if (d3_VecEqual(cam + 0x4c, pos, 0.1f) == 0 || d3_VecEqual(ai_Unkfc2dec + 0x84, dir, 0.001f) == 0) {
                d3_SetObjectPos(ai_Unkfc2dec, pos);
                d3_SetObjectRot(ai_Unkfc2dec, dir);
                rec = gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                if (rec != 0) {
                    *(float *)(rec + 0x88) = sv_CameraAngleFactor;
                    CAMLOG(rec);
                }
            }
            cam = ai_Unkfc2dec;
        }
        if (cam != 0) {
            if (ai_Unk6f23a0 == 0 && dm_L.wup - dm_L.wdown != 0) {
                dz = (dm_L.wup - dm_L.wdown) * 0.1 + sv_CameraHeightFactor;
                sv_CameraHeightFactor = (float)dz;
                if (1.0 < dz)
                    dz = 1.0;
                if (dz < 0.0)
                    dz = 0.0;
                sv_PlaceCamera((float)dz);
                rec = gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                cam = ai_Unkfc2dec;
                if (rec != 0) {
                    *(float *)(rec + 0x88) = sv_CameraAngleFactor;
                    CAMLOG(rec);
                    cam = ai_Unkfc2dec;
                }
            }
            if (cam != 0 && dm_L.rhold != 0 && dm_L.ldown == 0) {
                d3_MatTranspose(cam + 0x18c, right);
                v[0] = (float)(dm_P.x - ai_Unk6f1c88) * (float)ai_OptionScrollSpeed * 0.060606062f;
                v[1] = -(float)(dm_P.y - ai_Unk6f1c48) * (float)ai_OptionScrollSpeed * 0.060606062f;
                fwd[0] = right[0] * 0.0f + right[4] * 0.0f + right[8] * 1.0f;
                fwd[1] = 0.0f;
                fwd[2] = right[2] * 0.0f + right[6] * 0.0f + right[10] * 1.0f;
                if (0.1f <= sqrt(fwd[0] * fwd[0] + fwd[2] * fwd[2])) {
                    fwd[0] = right[0] * v[0] + right[4] * v[1];
                    fwd[2] = right[2] * v[0] + right[6] * v[1];
                } else {
                    ang = d3_VecAngle2((char *)(unsigned)&ai_AxisPair, fwd);
                    sn = (float)sin(-ang);
                    cs = (float)cos(-ang);
                    fwd[0] = (float)(v[1] * sn + v[0] * cs);
                    fwd[2] = (float)(v[1] * cs - v[0] * sn);
                }
                fwd[1] = 0.0f;
                dst[0] = fwd[0] + *(float *)(ai_Unkfc2dec + 0x4c);
                dst[1] = *(float *)(ai_Unkfc2dec + 0x50);
                dst[2] = fwd[2] + *(float *)(ai_Unkfc2dec + 0x54);
                if (d3_VecEqual(ai_Unk6f1c64, dst, 0.1f) == 0) {
                    m = *(float **)(ai_Unk5fb5b0 + 0xb0);
                    if (m != 0) {
                        lo = m[4] * 20.0f + m[0];
                        hi = (float)((int)m[8] - 0x14) * m[4] + m[0];
                        if (lo <= dst[0] && hi < dst[0])
                            lo = hi;
                        else if (lo <= dst[0])
                            lo = dst[0];
                        dst[0] = lo;
                        lo = m[6] * 20.0f + m[2];
                        hi = (float)((int)m[8] - 0x14) * m[6] + m[2];
                        if (dst[2] <= hi)
                            dst[2] = hi;
                        if (lo <= dst[2])
                            dst[2] = lo;
                    }
                    d3_SetObjectPos(ai_Unkfc2dec, dst);
                    rec = gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                    if (rec != 0)
                        CAMLOG(rec);
                }
                ai_Unk6f1c64[0] = dst[0];
                ai_Unk6f1c64[1] = dst[1];
                ai_Unk6f1c64[2] = dst[2];
                ai_Unk6f1c88 = (int)dm_P.x;
                ai_Unk6f1c48 = (int)dm_P.y;
                ai_Unk6f23a4 = 1;
            }
        }
    }
    return (ai_Unk6f23a4 - dm_L.wdown) + dm_L.wup + ai_Unk6f23a0;
}
