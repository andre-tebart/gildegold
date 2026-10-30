/* 0x5dd100, solves the cannon aim (727 B; am.c's 0x4a67a0 calls it): the ball flies from `from` to `to`
   (world units, 0.0254 m each) under gravity; bisects the launch speed (range[0] .. range[1]) and the
   elevation angle (range[2] .. range[3], radians) in 16 steps until the landing point is within 2.5 m of the
   target. The error is the flight time (vertical speed / g plus the fall from the height difference) times the
   horizontal speed minus the horizontal distance; too short raises the angle (the speed once the angle is at
   its maximum), too far lowers the speed (the angle once the speed is at its minimum). Returns 0 with the
   speed in *outSpeed, the elevation in *outElev, the heading angle (d3_VecAngle2 against ai_AxisPair) in
   *outYaw and three vectors: outDir the horizontal direction tilted by the elevation, outUp d3_YAxis tilted
   the same way, outSide their cross product; -1 when no setting in the range hits (reasoned: the names follow
   the use in am.c). @deep body complete and read against the original (bisection of speed range[0..1] / elevation range[2..3], 16 steps, +-2.5 m window, outputs outElev/outSpeed/outDir/outUp/outSide; conditions in the original's > / < jump forms, D3VECTOR4 dir/hdir give the 0x34 frame). Left (208 ins, all allocation): the original keeps speed/ang/speedStep/angStep on the FPU stack from before the loop (fld [ebx+0xc]; fld [ebx]; fld [g]...) and holds the sign in ebp / the counter in ecx with g, 2g, 1/g, drop, horiz*0.0254 in the slots 0x14/0x20/0x48/0x18/0x10; ours loads speed through an int copy (mov edx,[ebx]; mov [esp+0x48],edx), keeps sign in ecx, counter in edx and orders the three FPU temps differently (asmdiff hunks at 0x5dd108, 0x5dd1f7, 0x5dd294 fpu stack order, 0x5dd2d0, 0x5dd383 the cross-product operand order); horiz*0.0254 compiles fld mem; fmul const where the original has fld const; fmul mem */

#include <math.h>

int ai_SolveCannonAim(float *from, float *to, float *range, float *outElev, float *outYaw, float *outSpeed,
                      float *outDir, float *outUp, float *outSide)
{
    float speedStep = (range[1] - range[0]) * 0.5f;
    float angStep = (range[3] - range[2]) * 0.5f;
    float g = ai_Gravity;
    D3VECTOR4 dir, hdir;
    float horiz, drop, inv, s, c, h, t, err;
    float speed, ang;
    int sign, i;

    dir.x = to[0] - from[0];
    dir.y = to[1] - from[1];
    dir.z = to[2] - from[2];
    hdir.x = dir.x;
    hdir.y = 0.0f;
    hdir.z = dir.z;
    horiz = (float)sqrt(dir.x * dir.x + dir.z * dir.z);
    d3_Normalize(&dir);
    d3_Normalize(&hdir);
    *outYaw = d3_VecAngle2((D3VECTOR *)ai_AxisPair, (D3VECTOR *)&dir);
    drop = (float)fabs(to[1] - from[1]) * 0.0254f;
    horiz = 0.0254f * horiz;
    sign = 0;
    if (from[1] > to[1])
        sign = 1;
    else if (from[1] < to[1])
        sign = -1;
    speed = range[0];
    ang = range[3];
    inv = 1.0f / g;
    for (i = 0; i < 16; i++) {
        s = (float)sin(ang);
        c = (float)cos(ang);
        h = s * s * speed * speed / (g * 2.0f);
        t = inv * s * speed;
        if (sign == 1) {
            h = h + drop;
            t = (float)sqrt((h * inv + h * inv)) + t;
        } else if (sign == -1) {
            h = h - drop;
            t = (float)sqrt((h * inv + h * inv)) + t;
        }
        err = t * c * speed - horiz;
        if (err > 2.5f) {
            if (speed > range[0]) {
                speed = speed - speedStep;
                speedStep = speedStep * 0.5f;
            } else if (ang > range[2]) {
                ang = ang - angStep;
                angStep = angStep * 0.5f;
            } else
                return -1;
        } else if (err < -2.5f) {
            if (ang < range[3]) {
                ang = ang + angStep;
                angStep = angStep * 0.5f;
            } else if (speed < range[1]) {
                speed = speed + speedStep;
                speedStep = speedStep * 0.5f;
            } else
                return -1;
        } else {
            *outSpeed = speed;
            *outElev = ang;
            outDir[0] = hdir.x;
            outDir[1] = c * hdir.y - s * hdir.z;
            outDir[2] = s * hdir.y + c * hdir.z;
            outUp[0] = d3_YAxis.x;
            outUp[1] = d3_YAxis.y * c - d3_YAxis.z * s;
            outUp[2] = d3_YAxis.z * c + d3_YAxis.y * s;
            outSide[0] = outUp[2] * outDir[1] - outDir[2] * outUp[1];
            outSide[1] = outDir[2] * outUp[0] - outUp[2] * outDir[0];
            outSide[2] = outDir[0] * outUp[1] - outDir[1] * outUp[0];
            return 0;
        }
    }
    return -1;
}
