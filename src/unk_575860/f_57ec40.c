/* 0x57ec40, the cannon aiming update (1169 B): sc = the scene struct, dt = the frame time. Returns at once with
   sc + 0x7c set; the zoom sc + 0x20 moves by -dt * 1.6 (or +dt * 0.8 with the byte 0x14681d8) and is clamped to
   0 .. 1; the yaw sc + 0x14 and the pitch sc + 0x10 steer with the mouse: not dragging (0x1467390 = 0) -> the
   keys (0x1468288 / 0x1468290 the yaw, 0x146828b / 0x146828d the pitch, the screen 0xfb9070 / 0xfb9072 as the
   edges) drive the speeds sc + 0x54 / 0x58 (toward 0, or +/-1 at speed 0.25 / 0.8) into the deltas sc + 0x5c
   (* 1.5) / sc + 0x60 (* 0.8); dragging (0x1467390): inside the middle of the screen (0x50 .. 0xfb9070 - 0x50)
   the drag state sc + 0x88: 0 -> the start sc + 0x80 / sc + 0x84, the speeds zeroed; 1 -> the pitch -= (the
   old mouse y - sc + 0x84) * 0.008333334, the yaw -= (the mouse x - sc + 0x80) * 0.011111111; outside, or with
   the button released (the state 1 -> 0), the speeds fall toward 0. The pitch is clamped to -0.2617994 ..
   0.7853982 (the speed sc + 0x58 zeroed at the stop) and the yaw set from 0x5de73a (float args lost by Ghidra;
   the CRT atan-like helper) */

double ai_Unk5de73a();

void ai_Unk57ec40(char *sc, float dt)
{
    float f;
    int a;
    int b;
    int m;
    short y;

    if (*(int *)(sc + 0x7c))
        return;
    if (*(char *)((unsigned)&kb_Down + 0x18) == 0)
        f = *(float *)(sc + 0x20) - dt * 1.6f;
    else
        f = dt * 0.8f + *(float *)(sc + 0x20);
    *(float *)(sc + 0x20) = f;
    if (1.0f < *(float *)(sc + 0x20) || 0.0f < *(float *)(sc + 0x20)) {
        if (*(float *)(sc + 0x20) <= 1.0f)
            f = *(float *)(sc + 0x20);
        else
            f = 1.0f;
    } else {
        f = 0.0f;
    }
    *(float *)(sc + 0x20) = f;
    y = *(short *)((unsigned)&dm_L + 0x2);
    if (*(int *)((unsigned)&dm_L + 0x10) == 0) {
        if (*(int *)(sc + 0x88) == 1) {
            *(int *)(sc + 0x88) = 0;
            goto clamp;
        }
    } else {
        if (y < 0x50 || (unsigned)&d2_ScreenH - 0x50 < y) {
            *(int *)(sc + 0x88) = 0;
            goto clamp;
        }
        if (*(int *)(sc + 0x88) == 0) {
            *(int *)(sc + 0x80) = *(short *)(unsigned)&dm_L;
            *(int *)(sc + 0x84) = y;
            *(int *)(sc + 0x88) = 1;
            *(float *)(sc + 0x60) = 0;
            *(float *)(sc + 0x5c) = 0;
            *(float *)(sc + 0x58) = 0;
            *(float *)(sc + 0x54) = 0;
            goto clamp;
        }
        if (*(int *)(sc + 0x88) != 1) {
            a = 0;
            b = 0;
            goto steer;
        }
        a = *(short *)(unsigned)&dm_L;
        b = *(int *)(sc + 0x80);
        *(int *)(sc + 0x80) = a;
        m = *(int *)(sc + 0x84);
        y = *(short *)((unsigned)&dm_L + 0x2);
        *(int *)(sc + 0x84) = y;
        *(float *)(sc + 0x10) = *(float *)(sc + 0x10) - (float)(y - m) * 0.008333334f;
        *(float *)(sc + 0x14) = *(float *)(sc + 0x14) - (float)(a - b) * 0.011111111f;
        goto done;
    }
steer:
    a = 0;
    b = 0;
    if (*(char *)((unsigned)&kb_Down + 0xc8) != 0 || y < 6)
        a = 1;
    if (*(char *)((unsigned)&kb_Down + 0xd0) != 0 || (int)y >= (unsigned)&d2_ScreenH - 5)
        a--;
    if (*(char *)((unsigned)&kb_Down + 0xcb) != 0 || *(short *)(unsigned)&dm_L < 6)
        b = 1;
    if (*(char *)((unsigned)&kb_Down + 0xcd) != 0 || (int)*(short *)(unsigned)&dm_L >= (unsigned)&d2_ScreenW - 10)
        b--;
    if (*(float *)(sc + 0x54) == 0.0f) {
        if (b != 0)
            goto pitch_speed;
    } else if (b == 0) {
        if (*(float *)(sc + 0x54) >= 0.0f) {
            *(float *)(sc + 0x54) -= dt * 0.8f;
            if (*(float *)(sc + 0x54) <= 0.0f)
                *(float *)(sc + 0x54) = 0.0f;
        } else {
            *(float *)(sc + 0x54) += dt * 0.8f;
            if (*(float *)(sc + 0x54) >= 0.0f)
                *(float *)(sc + 0x54) = 0.0f;
        }
    } else {
pitch_speed:
        if (b < 1) {
            f = *(float *)(sc + 0x54) <= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x54) -= dt * f;
            if (*(float *)(sc + 0x54) <= -1.0f)
                *(float *)(sc + 0x54) = -1.0f;
        } else {
            f = *(float *)(sc + 0x54) >= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x54) += dt * f;
            if (*(float *)(sc + 0x54) >= 1.0f)
                *(float *)(sc + 0x54) = 1.0f;
        }
    }
    *(float *)(sc + 0x5c) = dt * *(float *)(sc + 0x54) * 1.5f;
    if (*(float *)(sc + 0x58) == 0.0f) {
        if (a != 0)
            goto yaw_speed;
    } else if (a == 0) {
        if (*(float *)(sc + 0x58) >= 0.0f) {
            *(float *)(sc + 0x58) -= dt * 0.8f;
            if (*(float *)(sc + 0x58) <= 0.0f)
                *(float *)(sc + 0x58) = 0.0f;
        } else {
            *(float *)(sc + 0x58) += dt * 0.8f;
            if (*(float *)(sc + 0x58) >= 0.0f)
                *(float *)(sc + 0x58) = 0.0f;
        }
    } else {
yaw_speed:
        if (a < 1) {
            f = *(float *)(sc + 0x58) <= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x58) -= dt * f;
            if (*(float *)(sc + 0x58) <= -1.0f)
                *(float *)(sc + 0x58) = -1.0f;
        } else {
            f = *(float *)(sc + 0x58) >= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x58) += dt * f;
            if (*(float *)(sc + 0x58) >= 1.0f)
                *(float *)(sc + 0x58) = 1.0f;
        }
    }
    *(float *)(sc + 0x10) += *(float *)(sc + 0x60);
    *(float *)(sc + 0x14) += *(float *)(sc + 0x5c);
done:
clamp:
    if (*(float *)(sc + 0x10) < -0.2617994f || 0.7853982f < *(float *)(sc + 0x10)) {
        if (*(float *)(sc + 0x10) < -0.2617994f)
            f = -0.2617994f;
        else
            f = *(float *)(sc + 0x10);
        if (f >= 0.7853982f)
            f = 0.7853982f;
        *(float *)(sc + 0x10) = f;
        *(float *)(sc + 0x58) = 0;
    }
    *(float *)(sc + 0x14) = (float)ai_Unk5de73a();
}
