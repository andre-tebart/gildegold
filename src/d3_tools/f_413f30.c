/* 0x413f30. The parentheses around drop->pos.w and the pos.y increment set VC6's operand ranks
   (load order and add association); the partial initializer of fall only restores the wind/fall
   slot order they disturb. See notes/vc6-matching.md. */

void d3t_UpdateRain(D3TRAIN *rain, float dt)
{
    D3VECTOR4 d, o, move, wind, fall = {0.0f, -0.75f}, vel;
    D3MATRIX m;
    float ex, ey, ez, wx, wz;
    float hw, hh, size, cx, cy, scale, f;
    D3TRAINDROP *drop;
    int i;

    d.x = rain->campos.x - d3_Camera->rotation.x;
    d.y = rain->campos.y - d3_Camera->rotation.y;
    d.z = rain->campos.z - d3_Camera->rotation.z;
    d3_MatFromAngles(&d, &m);
    rain->campos.x = d3_Camera->rotation.x;
    rain->campos.y = d3_Camera->rotation.y;
    rain->campos.z = d3_Camera->rotation.z;
    o.x = 0.0f * m._11 + 0.0f * m._21 + 2.5f * m._31;
    o.y = 0.0f * m._12 + 0.0f * m._22 + 2.5f * m._32;

    ex = rain->cam04c.x - d3_Camera->pos.x;
    ey = rain->cam04c.y - d3_Camera->pos.y;
    ez = rain->cam04c.z - d3_Camera->pos.z;
    move.x = ex * d3_Camera->matrix._11 + ey * d3_Camera->matrix._21 + ez * d3_Camera->matrix._31;
    move.y = ex * d3_Camera->matrix._12 + ey * d3_Camera->matrix._22 + ez * d3_Camera->matrix._32;
    move.z = ex * d3_Camera->matrix._13 + ey * d3_Camera->matrix._23 + ez * d3_Camera->matrix._33;
    move.x *= 0.0025f;
    move.y *= 0.0025f;
    move.z *= 0.0025f;
    rain->cam04c.x = d3_Camera->pos.x;
    rain->cam04c.y = d3_Camera->pos.y;
    rain->cam04c.z = d3_Camera->pos.z;

    wx = rain->windx;
    wz = rain->windz;
    wind.x = wx * d3_Camera->matrix._11 + 0.0f * d3_Camera->matrix._21 + wz * d3_Camera->matrix._31;
    wind.y = wx * d3_Camera->matrix._12 + 0.0f * d3_Camera->matrix._22 + wz * d3_Camera->matrix._32;
    wind.z = wx * d3_Camera->matrix._13 + 0.0f * d3_Camera->matrix._23 + wz * d3_Camera->matrix._33;
    fall.x = 0.0f * d3_Camera->matrix._11 + -0.75f * d3_Camera->matrix._21 + 0.0f * d3_Camera->matrix._31;
    fall.y = 0.0f * d3_Camera->matrix._12 + -0.75f * d3_Camera->matrix._22 + 0.0f * d3_Camera->matrix._32;
    fall.z = 0.0f * d3_Camera->matrix._13 + -0.75f * d3_Camera->matrix._23 + 0.0f * d3_Camera->matrix._33;

    hw = (d3_Viewport.right - d3_Viewport.left) >> 1;
    hh = (d3_Viewport.bottom - d3_Viewport.top) >> 1;
    size = hw > hh ? hw : hh;
    cx = d3_Viewport.left + hw;
    cy = d3_Viewport.top + hh;

    drop = rain->drops;
    for (i = 0; i < rain->count; drop++, i++) {
        vel.x = wind.x * drop->windScale + fall.x * drop->fallScale;
        drop->pos.x = vel.x * dt + drop->pos.x + move.x + o.x;
        if (drop->pos.x < -1000.0 || drop->pos.x > 1000.0) {
            char buf[128];
            sprintf(buf, "Drop %li: X is %f", i, drop->pos.x);
            sprintf(buf, "    Rain->WindX: %f    Rain->WindZ: %f", rain->windx, rain->windz);
            drop->pos.x = 0.0f;
        }
        while (drop->pos.x < -1.0)
            drop->pos.x += 2.0;
        while (drop->pos.x >= 1.0)
            drop->pos.x -= 2.0;

        vel.y = wind.y * drop->windScale + fall.y * drop->fallScale;
        drop->pos.y += (vel.y * dt + move.y + o.y);
        if (drop->pos.y < -1000.0 || drop->pos.y > 1000.0) {
            char buf[128];
            sprintf(buf, "Drop %li: Y is %f", i, drop->pos.y);
            sprintf(buf, "    Rain->WindX: %f    Rain->WindZ: %f", rain->windx, rain->windz);
            drop->pos.y = 0.0f;
        }
        while (drop->pos.y < -1.0)
            drop->pos.y += 2.0;
        while (drop->pos.y >= 1.0)
            drop->pos.y -= 2.0;

        vel.z = wind.z * drop->windScale + fall.z * drop->fallScale;
        drop->pos.z += vel.z * dt + move.z * 2.0f;
        if (drop->pos.z < -1000.0 || drop->pos.z > 1000.0) {
            char buf[128];
            sprintf(buf, "Drop %li: Z is %f", i, drop->pos.z);
            sprintf(buf, "    Rain->WindX: %f    Rain->WindZ: %f", rain->windx, rain->windz);
            drop->pos.z = 0.0f;
        }
        while (drop->pos.z < -1.0)
            drop->pos.z += 2.0;
        while (drop->pos.z >= 1.0)
            drop->pos.z -= 2.0;

        scale = (rain->count / 2.0f + size / 3.0f) * 3.0f;
        f = scale / (drop->pos.z * 2.0f + 3.0f);
        drop->sx1 = drop->pos.x * f + cx;
        drop->sy1 = cy - drop->pos.y * f;
        vel.x *= (drop->pos.w);
        vel.y *= (drop->pos.w);
        f = scale / (((drop->pos.w) * vel.z + drop->pos.z) * 2.0f + 3.0f);
        drop->sx2 = (vel.x + drop->pos.x) * f + cx;
        drop->sy2 = cy - (vel.y + drop->pos.y) * f;
    }
}
