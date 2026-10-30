/* 0x5263a0, the camera scroll step of one frame: the border zone of the mouse (0x1467380 / 82 against the window 0x6310b0 / 0x6310b4 .. 0x5f3238 / 0x5f323c, margin 0x40, or the arrow keys 0x146828b / d / 0x1468290 / 88)
   accelerates the scroll speeds 0x6f2388 (x) / 0x6f2390 (y) by 5 * dt (dt = 0.4 * the elapsed ms, at most 4.0) up to +-50 starting at +-10, picks the direction code 0x6ed9d0 (1 .. 8, 5 .. 8 the corners) and decays
   the speeds otherwise; the camera is then moved (0x43b790) by the speed rotated into the view direction (0x453b40, sin / cos), clamped to the map rectangle (0x5fb5b0 + 0xb0) and refused near the point 0x6f1c74;
   returns 1 when the camera moved. ROUGH */

int sv_Unk5263a0(void)
{
    int moved, my;
    double dt;
    float k, f;
    float pos[3], dst[3], v[3], right[16], fwd[3];   /* right is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    float a, sn, cs;
    float *m;
    float lo, hi;
    int dirs;

    moved = 0;
    if (d2_PointerLook != 0 && d2_PointerLook < 9)
        d2_PointerLook = 0;
    if (d3t_CamMove != 0 || ai_Unk6310e4 != 0)
        return 0;
    if (ai_Unk6f2394 == 0)
        dt = 1.0;
    else
        dt = (unsigned)(d3_Time - ai_Unk6f2394) * 0.4;
    if (4.0 <= dt)
        dt = 4.0;
    ai_Unk6f2394 = d3_Time;
    my = dm_L.y;
    f = (float)(5.0 * dt);
    dirs = 0;
    if ((my == di_MinY && dm_L.x < di_MinX + 0x40) || (my < di_MinY + 0x40 && dm_L.x == di_MinX)) {
        if (ai_Unk6f2390 == 0.0f)
            ai_Unk6f2390 = 10.0f;
        if (ai_Unk6f2390 < 50.0f)
            ai_Unk6f2390 += f;
        if (ai_Unk6f2388 == 0.0f)
            ai_Unk6f2388 = -10.0f;
        if (-50.0f < ai_Unk6f2388)
            ai_Unk6f2388 -= f;
        d2_PointerLook = 5;
    } else if ((my == di_MinY && di_MaxX - 0x40 < dm_L.x) || (my < di_MinY + 0x40 && dm_L.x == di_MaxX)) {
        if (ai_Unk6f2390 == 0.0f)
            ai_Unk6f2390 = 10.0f;
        if (ai_Unk6f2390 < 50.0f)
            ai_Unk6f2390 += f;
        if (ai_Unk6f2388 == 0.0f)
            ai_Unk6f2388 = 10.0f;
        if (ai_Unk6f2388 < 50.0f)
            ai_Unk6f2388 += f;
        d2_PointerLook = 6;
    } else if ((my == di_MaxY && di_MaxX - 0x40 < dm_L.x) || (di_MaxY - 0x40 < my && dm_L.x == di_MaxX)) {
        if (ai_Unk6f2390 == 0.0f)
            ai_Unk6f2390 = -10.0f;
        if (-50.0f < ai_Unk6f2390)
            ai_Unk6f2390 -= f;
        if (ai_Unk6f2388 == 0.0f)
            ai_Unk6f2388 = 10.0f;
        if (ai_Unk6f2388 < 50.0f)
            ai_Unk6f2388 += f;
        d2_PointerLook = 8;
    } else if ((my == di_MaxY && dm_L.x < di_MinX + 0x40) || (di_MaxY - 0x40 < my && dm_L.x == di_MinX)) {
        if (ai_Unk6f2390 == 0.0f)
            ai_Unk6f2390 = -10.0f;
        if (-50.0f < ai_Unk6f2390)
            ai_Unk6f2390 -= f;
        if (ai_Unk6f2388 == 0.0f)
            ai_Unk6f2388 = -10.0f;
        if (-50.0f < ai_Unk6f2388)
            ai_Unk6f2388 -= f;
        d2_PointerLook = 7;
    } else {
        if (dm_L.x == di_MinX || kb_Down[0xcb] != 0) {
            if (ai_Unk6f2388 == 0.0f)
                ai_Unk6f2388 = -10.0f;
            if (-50.0f < ai_Unk6f2388)
                ai_Unk6f2388 -= f;
            d2_PointerLook = 4;
        } else if (dm_L.x == di_MaxX || kb_Down[0xcd] != 0) {
            if (ai_Unk6f2388 == 0.0f)
                ai_Unk6f2388 = 10.0f;
            if (ai_Unk6f2388 < 50.0f)
                ai_Unk6f2388 += f;
            d2_PointerLook = 3;
        } else {
            if (my == di_MaxY || kb_Down[0xd0] != 0) {
                if (ai_Unk6f2390 == 0.0f)
                    ai_Unk6f2390 = -10.0f;
                if (-50.0f < ai_Unk6f2390)
                    ai_Unk6f2390 -= f;
                dirs = 1;
                d2_PointerLook = 1;
            } else if (my == di_MinY || kb_Down[0xc8] != 0) {
                if (ai_Unk6f2390 == 0.0f)
                    ai_Unk6f2390 = 10.0f;
                if (ai_Unk6f2390 < 50.0f)
                    ai_Unk6f2390 += f;
                dirs = 1;
                d2_PointerLook = 2;
            }
            if (ai_Unk6f2388 <= 0.0f) {
                if (ai_Unk6f2388 < 0.0f) {
                    ai_Unk6f2388 += f;
                    if (0.0f < ai_Unk6f2388)
                        ai_Unk6f2388 = 0.0f;
                }
            } else {
                ai_Unk6f2388 -= f;
                if (ai_Unk6f2388 < 0.0f)
                    ai_Unk6f2388 = 0.0f;
            }
            if (dirs)
                goto scroll;
        }
        if (ai_Unk6f2390 <= 0.0f) {
            if (!(0.0f <= ai_Unk6f2390) && (ai_Unk6f2390 += f, ai_Unk6f2390 > 0.0f))
                ai_Unk6f2390 = 0.0f;
        } else {
            ai_Unk6f2390 -= f;
            if (!(0.0f <= ai_Unk6f2390))
                ai_Unk6f2390 = 0.0f;
        }
    }
scroll:
    if (ai_Unkfc2dec != 0 && (ai_Unk6f2388 != 0.0f || ai_Unk6f2390 != 0.0f)) {
        if (ai_FallbackBuilding == 0) {
            if (ai_PlayerBuilding != 0)
                dt *= 0.25;
        } else
            dt *= 0.5;
        k = (float)(ai_Unk623818 * dt * (sv_CameraHeightFactor * 1.5 + 0.6000000238418579));
        d3_MatTranspose(ai_Unkfc2dec + 0x18c, right);
        v[0] = ai_Unk6f2388 * k * 1.1f;
        v[1] = ai_Unk6f2390 * k * 1.1f;
        fwd[0] = right[0] * 0.0f + right[4] * 0.0f + right[8] * 1.0f;
        fwd[1] = 0.0f;
        fwd[2] = right[2] * 0.0f + right[6] * 0.0f + right[10] * 1.0f;
        if (0.1f <= sqrt(fwd[0] * fwd[0] + fwd[2] * fwd[2])) {
            fwd[0] = right[0] * v[0] + right[4] * v[1];
            fwd[2] = right[2] * v[0] + right[6] * v[1];
        } else {
            a = d3_VecAngle2((char *)(unsigned)&ai_AxisPair, fwd);
            sn = (float)sin(-a);
            cs = (float)cos(-a);
            fwd[0] = (float)(v[1] * sn + v[0] * cs);
            fwd[2] = (float)(v[1] * cs - v[0] * sn);
        }
        dst[0] = fwd[0] + *(float *)(ai_Unkfc2dec + 0x4c);
        dst[1] = *(float *)(ai_Unkfc2dec + 0x50);
        dst[2] = fwd[2] + *(float *)(ai_Unkfc2dec + 0x54);
        if (d3_VecEqual(ai_Unk6f1c74, dst, 0.1f) == 0) {
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
            moved = 1;
        }
        ai_Unk6f1c74[0] = fwd[0];
        ai_Unk6f1c74[1] = 0.0f;
        ai_Unk6f1c74[2] = fwd[2];
    }
    ai_Unk6f238c = ai_Unk6f2388;
    ai_Unk6f1c44 = ai_Unk6f2390;
    return moved;
}
